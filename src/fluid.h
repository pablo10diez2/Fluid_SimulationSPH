#ifndef FLUID_H_
#define FLUID_H_

#include <iostream>
#include "config.h"
#include "gravity.h"
#include <unordered_map>
#include <vector>
#include <cmath>

struct pairHash{
    std::size_t operator()(const std::pair<int, int>& p) const {
        std::size_t h1 = std::hash<int>()(p.first);
        std::size_t h2 = std::hash<int>()(p.second);
        return h1 ^ (h2 << 1);
    }
};

void findNeighbors(std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, int numCircles);
std::vector<int> findNextIndices(std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, int x, int y);
void calculateDensities(int numcircles, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, std::vector<float>& densities);
void calculatePressures(int numCircles, std::vector<float>& pressures, std::vector<float>& densities);
void calculatePressureForce(int numCircles, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, std::vector<float>& pressureForces, std::vector<float>& pressures, std::vector<float>& densities);
std::vector<float> kernelSpikyDerived(float distance, int circle1, int circle2, std::vector<float>& centers);
float kernelPoly6(float distance);
void calculateViscosity(int numCircles, std::vector<float>& viscosities, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, std::vector<float>& speeds, std::vector<float>& densities);
float kernelViscosityLaplacian(float distance);
void applyForces(int numCircles, float timeDiffG, std::vector<float>& centers, std::vector<float>& speeds, std::vector<float>& pressureForces, std::vector<float>& viscosities);
#endif
