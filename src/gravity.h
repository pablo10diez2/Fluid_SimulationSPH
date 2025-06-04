#include <cmath>
#include <vector>
#include <iostream>
#include "globals.h"

void gravity(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds);

void isEdge(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds);
void ballCollisions(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds);
