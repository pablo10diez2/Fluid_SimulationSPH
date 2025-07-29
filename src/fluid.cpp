#include "fluid.h"

void findNeighbors(std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, int numCircles){
    grid.clear();
    
    for(int i=0; i<numCircles; i++){
        int ix = floor((1+centers[2*i])/h);
        int iy = floor((1+centers[2*i+1])/h);
        grid[{ix, iy}].push_back(i);
    }
}

std::vector<int> findNextindices(std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, int x, int y){
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

        std::vector<int> neighbors = findNextindices(centers, grid, ix, iy);
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

float kernelPoly6(float distance){
    float result = 0;
    
    if(distance >= 0 && distance <= h){
        result = (h*h)-(distance*distance);
        result = pow(result, 3);
        result *= 315/(64*M_PI*pow(h, 9));
    }else{
        return 0;
    }
    return result;
}
