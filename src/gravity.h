#include <cmath>
#include <vector>
#include <iostream>
#include "globals.h"

void gravity(std::vector<float>& centers, int numCircles, std::vector<float>& speeds, float timeDiff);
void isEdge(std::vector<float>& centers, int numCircles, std::vector<float>& speeds);
void ballCollisions(std::vector<float>& centers, int numCircles, std::vector<float>& speeds);
float getDistance(std::vector<float>& centers, int circle1, int circle2);

