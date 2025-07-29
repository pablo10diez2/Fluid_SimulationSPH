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

void calculateDensities(int numCircles, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, std::vector<float>& densities){

    densities.resize(numCircles);

    for(int i=0; i<numCircles; i++){
        float density = 0;
        int ix = floor((1+centers[2*i])/h);
        int iy = floor((1+centers[2*i+1])/h);

        std::vector<int> neighbors = findNextIndices(centers, grid, ix, iy);
        for(int j : neighbors){
            float distance = getDistance(centers, i, j);
            density += kernelPoly6(distance);
        }
        densities[i] = density;
    }
}

void calculatePressures(int numCircles, std::vector<float>& pressures, std::vector<float>& densities){
    const float k = 8;
    const float restDensity = 1;

    pressures.resize(numCircles);

    for(int i=0; i<numCircles; i++){
        pressures[i] = k*(densities[i]-restDensity);
    }
}

void calculatePessureForce(int numCircles, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, std::vector<float>& pressureForces, std::vector<float>& pressures, std::vector<float>& densities){
    for(int i=0; i<numCircles; i++){
        std::vector<float> pressureForce(2, 0.0f);

        int ix = floor((1+centers[2*i])/h);
        int iy = floor((1+centers[2*i+1])/h);

        std::vector<int> neighbors = findNextIndices(centers, grid, ix, iy); 
        for(int j: neighbors){
            float distance = getDistance(centers, i, j);

            std::vector<float> gradW = kernelSpikyDerived(distance, i, j, centers);
            float calc = (pressures[i]+pressures[j])/2*densities[j];

            pressureForce[0] += calc * gradW[0];
            pressureForce[1] += calc * gradW[1];
        }
        
        pressureForces[2*i] = -pressureForce[0];
        pressureForces[2*i+1] = -pressureForce[1];
    }
}

float kernelPoly6(float distance){
    if(distance > 0 && distance <= h){
        float result = (h*h)-(distance*distance);
        result = pow(result, 3);
        result *= 315/(64*M_PI*pow(h, 9));
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
