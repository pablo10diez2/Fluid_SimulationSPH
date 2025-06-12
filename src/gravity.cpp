#include "gravity.h"
#include <cmath>

std::vector<float> center;

void gravity(std::vector<float>& centers, int numCircles, std::vector<float>& speeds){
    for(int i=0; i<numCircles; i++){
        if(speeds[i] != 0.0f){
            float acc = 0.0001f;
            float calculo = speeds[i]-acc;
            speeds[i] = calculo;
            centers[2*i+1] = centers[2*i+1] + speeds[i];
        }
    }
}

void isEdge(std::vector<float>& centers, int numCircles, std::vector<float>& speeds){
    for(int i=0; i<numCircles; i++){
        if((centers[2*i+1]-radiusArray[i]) <= -0.9999f){
            speeds[i] = 0.58*std::abs(speeds[i]);

            if(speeds[i]<0.0001f){
                speeds[i] = 0.0f;
            }
        }
    }
}

void getCenters(std::vector<float>& vertices, int numTriangles, int numCircles, int circle){
    int index = circle*numTriangles*3*6;
    float xCenter = vertices[index]-radiusArray[circle];
    float yCenter = vertices[index+1];
    center.clear();
    center.push_back(xCenter);
    center.push_back(yCenter);
}

float getDistance(std::vector<float>& vertices, int numTriangles, int numCircles, int circle1, int circle2){
    getCenters(vertices, numTriangles, numCircles, circle1);
    float center1x = center[0];
    float center1y = center[1];

    getCenters(vertices, numTriangles, numCircles, circle2);
    float center2x = center[0];
    float center2y = center[1];
    
    float dx = center1x-center2x;
    float dy = center1y-center2y;
    
    float h = std::sqrt(pow(dx,2) + pow(dy,2));
    h = h -(radiusArray[circle1]+radiusArray[circle2]);
    return h;
}

void ballCollisions(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds){
    for(int i = 0; i<numCircles; i++){
        for(int z= 0; z<numCircles; z++){
            if(i!=z){
                float distance = getDistance(vertices, numTriangles, numCircles, i, z);
                if(distance < 0.001f){
                    std::cout<<"Collision: "<<i<<"-"<<z<<"Distance: "<<distance<<std::endl;
                    applyCollision(vertices, numTriangles, numCircles, speeds, i, 1);
                    applyCollision(vertices, numTriangles, numCircles, speeds, z, -1);
                }
            }   
        }
    }
}

void applyCollision(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds, int circle1, float sign){
    for(int z=0; z<3*6*numTriangles; z+=6){
        vertices[z+(numTriangles*circle1*3*6)] += 0.1f*sign;
    }
}
