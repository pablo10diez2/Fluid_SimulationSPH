#include "fluid.h"
void findNeighbors(std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, int numCircles){
    grid.clear();
    
    for(int i=0; i<numCircles; i++){
        int ix = floor((1+centers[2*i])/h);
        int iy = floor((1+centers[2*i+1])/h);
        grid[{ix, iy}].push_back(i);
    }
}

std::vector<int> findNextIndices(std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, int x, int y){
    std::vector<int> indices;

    for(int i=-1; i<2; i++){
        for(int j=-1; j<2; j++){
            int ix = x+i;
            int iy = y+j;
            auto found = grid.find({ix, iy});

            if(found != grid.end()){
                int numParticles = grid[{ix, iy}].size();
                for(int h=0; h<numParticles; h++){
                    indices.push_back(grid[{ix,iy}][h]);
                }
            }
        }
    }
    return indices;
}

void searchDistances(std::unordered_map<std::pair<int, int>, float, pairHash>& distances, int numCircles, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid){
    distances.clear();
    for(int i=0; i<numCircles; i++){
        int ix = floor((1+centers[2*i])/h);
        int iy = floor((1+centers[2*i+1])/h);

        std::vector<int> neighbors = findNextIndices(centers, grid, ix, iy); 
        for(int j: neighbors){
            distances[{i, j}] = getDistance(centers, i, j);
        }
    }
}

void calculateDensities(int numCircles, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, std::vector<float>& densities, std::unordered_map<std::pair<int, int>, float, pairHash>& distances){

    densities.assign(numCircles, 0.0f);

    for(int i=0; i<numCircles; i++){
        float density = 0;
        int ix = floor((1+centers[2*i])/h);
        int iy = floor((1+centers[2*i+1])/h);

        std::vector<int> neighbors = findNextIndices(centers, grid, ix, iy);
        for(int j : neighbors){
            float distance;
            if(i==j){
                distance = 0.0f;
            }else{
                distance = distances[{i, j}];
            }
            density += kernelPoly6(distance);
        }
        densities[i] = density;
    }
}

void calculatePressures(int numCircles, std::vector<float>& pressures, std::vector<float>& densities){
    const float k = 1.0f;
    const float restDensity = 1000.0f;

    pressures.resize(numCircles);

    for(int i=0; i<numCircles; i++){
        pressures[i] = k*(densities[i]/restDensity-1.0f);
    }
}

void calculatePressureForce(int numCircles, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, std::vector<float>& pressureForces, std::vector<float>& pressures, std::vector<float>& densities, std::unordered_map<std::pair<int, int>, float, pairHash>& distances){
    const float epsilon = 1e-4f;

    pressureForces.resize(centers.size());
    for(int i=0; i<numCircles; i++){
        std::vector<float> pressureForce(2, 0.0f);

        int ix = floor((1+centers[2*i])/h);
        int iy = floor((1+centers[2*i+1])/h);

        std::vector<int> neighbors = findNextIndices(centers, grid, ix, iy); 

        for(int j: neighbors){
            if(i != j){
                float distance = distances[{i, j}];
                std::vector<float> gradW = kernelSpikyDerived(distance, i, j, centers);

                if(densities[j]>epsilon){
                    float calc = (pressures[i]+pressures[j])/(2*densities[j]);
                    pressureForce[0] += calc * gradW[0];
                    pressureForce[1] += calc * gradW[1];
                }
            }
        }
        
        pressureForces[2*i] = -pressureForce[0];
        pressureForces[2*i+1] = -pressureForce[1];
    }
}

void calculateViscosity(int numCircles, std::vector<float>& viscosities, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, std::vector<float>& speeds, std::vector<float>& densities, std::unordered_map<std::pair<int, int>, float, pairHash>& distances){
    const float epsilon = 1e-6f;
    viscosities.resize(centers.size());

    for(int i=0; i<numCircles; i++){
        int ix = floor((1+centers[2*i])/h);
        int iy = floor((1+centers[2*i+1])/h);
        
        std::vector<float> viscosity(2, 0.0f);

        std::vector<int> neighbors = findNextIndices(centers, grid, ix, iy);
        for(int j: neighbors){
            if(i!=j){
                float distance = distances[{i, j}];
            
                if(densities[j]>epsilon){
                    viscosity[0] += ((speeds[2*j]-speeds[2*i])/densities[j])*kernelViscosityLaplacian(distance);
                    viscosity[1] += ((speeds[2*j+1]-speeds[2*i+1])/densities[j])*kernelViscosityLaplacian(distance);
                }
            }
        }
        viscosities[2*i] = 10.0f*viscosity[0];
        viscosities[2*i+1] =10.0f*viscosity[1];
    }
}

void applyForces(int numCircles, float timeDiffG, std::vector<float>& centers, std::vector<float>& speeds, std::vector<float>& pressureForces, std::vector<float>& viscosities, std::vector<float>& densities){
    const float g = -0.98f;

    for(int i=0; i<numCircles; i++){
        float density = std::max(densities[i], 0.0001f);
        float fx = (pressureForces[2*i] + viscosities[2*i])/density;
        float fy = (pressureForces[2*i+1] + viscosities[2*i+1] + g)/density;

        speeds[2*i] += fx*timeDiffG;
        speeds[2*i+1] += fy*timeDiffG;

        centers[2*i] += speeds[2*i] * timeDiffG;
        centers[2*i+1] += speeds[2*i+1] * timeDiffG;

    }
}

float kernelPoly6(float distance){
    if(distance >= 0 && distance <= h){
        float result = (h*h)-(distance*distance);
        result = pow(result, 3);
        result *= (315/(64*M_PI*pow(h, 9)));
        return result;
    }else{
        return 0;
    }
}

std::vector<float> kernelSpikyDerived(float distance, int circle1, int circle2, std::vector<float>& centers){
    std::vector<float> result(2, 0.0f);

    if(distance> 0 && distance <=h){
        float escalar = -45.0f/(M_PI*pow(h, 6)) * pow(h-distance, 2);
        float rX = (centers[2*circle1]-centers[2*circle2])/distance;
        float rY = (centers[2*circle1+1]-centers[2*circle2+1])/distance;
        
        result[0] = escalar * rX;
        result[1] = escalar * rY;
    }

    return result;
}

float kernelViscosityLaplacian(float distance){
    float result = 0.0f;
    
    if(distance>0 && distance<=h){
        result = 45*(h-distance)/(M_PI*pow(h,6));
        return result;
    }return 0.0f;
}
