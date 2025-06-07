#include <cmath>
#include <vector>
#include <iostream>
#include "globals.h"

void gravity(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds);

void isEdge(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds);

void ballCollisions(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds);

void getCenters(std::vector<float>& vertices, int numTriangles, int circle);

float getDistance(std::vector<float>& vertices, int numTriangles, int numCircles, int circle1, int circle2);
void applyCollision(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds, int circle1, float sign);
