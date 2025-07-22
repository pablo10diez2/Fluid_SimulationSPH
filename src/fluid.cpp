#include "fluid.h"

void findNeighbors(std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<float>, pairHash>& grid, int numCircles){
    grid.clear();
    
    for(int i=0; i<numCircles; i++){
        int ix = floor((1+centers[2*i])/h);
        int iy = floor((1+centers[2*i+1])/h);
        grid[{ix, iy}].push_back(i);
    }
}
