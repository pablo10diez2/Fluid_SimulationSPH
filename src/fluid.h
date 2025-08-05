#ifndef FLUID_H_
#define FLUID_H_

#include <iostream>
#include "config.h"
#include "gravity.h"
#include <unordered_map>
#include <vector>
#include <cmath>

struct pairHash {
    std::size_t operator()(const std::pair<int,int>& p) const {
        std::size_t seed = 0;
        seed ^= std::hash<int>()(p.first) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        seed ^= std::hash<int>()(p.second) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        return seed;
    }
};

void findNeighbors(std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, int numCircles);
std::vector<int> findNextIndices(std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, int x, int y);
void calculateDensities(int numcircles, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, std::vector<float>& densities, std::unordered_map<std::pair<int, int>, float, pairHash>& distances);
void calculatePressures(int numCircles, std::vector<float>& pressures, std::vector<float>& densities);
void calculatePressureForce(int numCircles, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, std::vector<float>& pressureForces, std::vector<float>& pressures, std::vector<float>& densities, std::unordered_map<std::pair<int, int>, float, pairHash>& distances);
std::vector<float> kernelSpikyDerived(float distance, int circle1, int circle2, std::vector<float>& centers);
float kernelPoly6(float distance);
void calculateViscosity(int numCircles, std::vector<float>& viscosities, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid, std::vector<float>& speeds, std::vector<float>& densities, std::unordered_map<std::pair<int, int>, float, pairHash>& distances);
float kernelViscosityLaplacian(float distance);
void applyForces(int numCircles, float timeDiffG, std::vector<float>& centers, std::vector<float>& speeds, std::vector<float>& pressureForces, std::vector<float>& viscosities, std::vector<float>& densities);
void searchDistances(std::unordered_map<std::pair<int, int>, float, pairHash>& distances, int numCircles, std::vector<float>& centers, std::unordered_map<std::pair<int, int>, std::vector<int>, pairHash>& grid);
#endif
