#ifndef GRAVITY_H_
#define GRAVITY_H_

#include <cmath>
#include <vector>
#include <iostream>
#include "config.h"

void isEdge(std::vector<float>& centers, int numCircles, std::vector<float>& speeds, float xSquare);
float getDistance(std::vector<float>& centers, int circle1, int circle2);

#endif
