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

float getDistance(std::vector<float>& centers, int circle1, int circle2){
    float dx = centers[circle2] - centers[circle1];
    float dy = centers[circle2+1] - centers[circle1+1];
    float distance = std::sqrt( pow(dx,2) + pow(dy,2) );
    distance = std::abs(distance);

    return distance-radiusArray[circle1] - radiusArray[circle2];
}

void ballCollisions(std::vector<float>& centers, int numCircles, std::vector<float>& speeds){
    for(int i=0; i<numCircles; i++){
        for(int z=0; z<numCircles; z++){
            if(i != z){
                float distance = getDistance(centers, i, z);
                if(distance < 0.00001f){
                    std::cout<<"Collision: "<<i<<"-"<<z<<"Distance: "<<distance<<std::endl;
                    //centers[i] += 0.1;
                    //centers[i+1] += 0.1;
                    //centers[z] += -0.1f;
                    //centers[z+1] += -0.1f;
                }
            }
        }
    }
}
