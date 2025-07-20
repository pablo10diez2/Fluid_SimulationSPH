#ifndef FLUID_H_
#define FLUID_H_

#include <iostream>
#include "config.h"
#include <unordered_map>
#include <vector>

void initializeGrid(std::unordered_map<float, std::vector<float>>* grid);
void findNeighbors(int index);

#endif
