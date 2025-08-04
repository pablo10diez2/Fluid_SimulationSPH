#include "fluid.h"

void findNeighbors(std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, int numCircles){
    for(int i=0; i<numCircles; i++){
        int ix = floor((1+centers[2*i])/h);
        int iy = floor((1+centers[2*i+1])/h);

        grid[{ix, iy}].push_back(i);
    }
}

std::vector<float> findNextIndices(std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, int x, int y){
    std::vector<int> indices;

    for(int i=-1; i<2; i++){
        for(int j=-1; j<2; j++){
            int ix = x+i;
            int iy = y+j;

            auto found = grid.find({ix, iy});

            if(found != grid.end()){
                int numParticles = grid[{ix, iy}].size();
                for(int h=0; h<numParticles; h++){
                    indices.push_back(grid[{ix, iy}][h]);
                }
            }
        }
    }
}

void searchDistances(std::unordered_map<std::pair<int, int>, float, pairHash>& distances, int numCircles, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid){
    for(int i=0; i<numCircles; i++){
        int ix= floor((1+centers[2*i])/h);
        int iy = floor((1+centers[2*i+1])/h);
        
        std::vector<float> neighbors = findNextIndices(grid, ix, iy);
        for(int j: neighbors){
            distances[{i, j}] = getDistance(centers, i, j);
        }
    }
}

void calculateDensities(int numCircles, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, std::vector<float>& densities, std::unordered_map<std::pair<int, int>, float, pairHash>& distances){
    for(int i=0; i<numCircles; i++){
        float density = 0.0f;
        int ix = floor((1+centers[2*i])/h);
        int iy = floor((1+centers[2*i+1])/h);

        std::vector<int> neighbors = findNextIndices(grid, ix, iy);
        
        for(int j: neighbors){
            float distance = distances[{i, j}];
            density += mass*kernelDensity(distance);
        }
    }
}

float kernelDensity(float distance){
    if(distance >0 && distance<= h){
        float result = 315/(64*M_PI*pow(h, 9));
        result *= pow(pow(h, 2)-pow(distance, 2), 3);
        return result;
    }return 0.0f;
}

void calculatePressures(int numCircles, std::vector<float>& pressures, std::vector<float>& densities){
    float k = 1.0f;
    float restDensity = 1000.0f;

    for(int i=0; i<numCirclesm i++){
        pressures[i] = k*(densities[i]-restDensity);
    }
}

void calculatePressureForce(){
    pressureForces.resize(centers.size());
    pressureForces.clear();

    for(int i=0; i<numCircles; i++){
        int ix = floor((1+centers[2*i])/h);
        int iy = floor((1+centers[2*i+1])/h);

        std::vector<float> pressureForce(2, 0.0f);
        
        std::vector<int> neighbors = findNextIndices(centers, ix, iy);
        for(int j: neighbors){
            if(i!=j){
                float distance = distances[{i, j}]; 
                std::vector<float> kernel = kernelPressureForce(distance, i, j, centers);

                if(densities > 0.00001f){
                    float calc = mass*((pressures[i]+pressures[j])/2*densities[j]);
                    pressureForce[0] += calc*kernel[0];
                    pressureForce[1] += calc*kernel[1];
                }
            }
        }
        pressureForces[2*i] = -pressureForce[0];
        pressureForces[2*i+1] = -pressureForce[1];
    }
}

std::vector<float> kernelPressureForce(float distance, int circle1, int circle2, std::vector<float>& centers){
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

void calculateViscosityForce(){
    
}
