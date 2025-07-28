#include "fluid.h"

void findNeighbors(std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, int numCircles){
    grid.clear();
    
    for(int i=0; i<numCircles; i++){
        int ix = floor((1+centers[2*i])/h);
        int iy = floor((1+centers[2*i+1])/h);
        grid[{ix, iy}].push_back(i);
    }
}

std::vector<int> findNextIndices(std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, std::pair<int, int>& pair, std::vector<int>& indices){
    std::vector<int> indices;

    for(int i=-1; i<2; i++){
        for(int j=-1; j<2; j++){
            int ix = pair.first+i;
            int iy = pair.second+j;
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

void calculateDensities(int numCircles){
    for(int i=0; i<numCircles; i++){
        float density = 0;
        std::vector<int> neighbors = findNextIndices();
        
        for(int j; neighbors){
            if(j != i){
                density += 0; //implementar
            }
        }
        densities.push_back(density);
    }
}
