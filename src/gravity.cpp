#include "gravity.h"
#include <cmath>

std::vector<float> center;

void gravity(std::vector<float>& centers, int numCircles, std::vector<float>& speeds, float timeDiff){
    float g = -1.5f;
    float ax = 0.99f;
    for(int i=0; i<numCircles; i++){
        if(speeds[2*i+1] != 0.0f){
            speeds[2*i+1] += g*timeDiff;
            centers[2*i+1] += speeds[2*i+1]*timeDiff;
        }

        if(std::abs(speeds[2*i])> 0.0001f){
            speeds[2*i] *= ax;
        }else{
            speeds[2*i] = 0.0f;
        }
        centers[2*i] += speeds[2*i] * timeDiff;
    }
}

void isEdge(std::vector<float>& centers, int numCircles, std::vector<float>& speeds){
    for(int i=0; i<numCircles; i++){
        float bottonY = centers[2*i+1]-radiusArray[i];
        float bottonX = centers[2*i]-radiusArray[i];
        float topY = centers[2*i+1]+radiusArray[i];
        float topX = centers[2*i]+radiusArray[i];
        
        if(bottonY<-0.999f){
            centers[2*i+1] = -0.999f + radiusArray[i];
            if(std::abs(speeds[2*i+1]) < 0.001f){
                speeds[2*i+1] = 0.0f;
            }else{
                speeds[2*i+1] = -0.55f * speeds[2*i+1];
            }
        }

        if(topY > 0.999f){
            centers[2*i+1] = 0.999f - radiusArray[i];
                speeds[2*i+1] = -0.55f*speeds[2*i+1];
        }

        if(bottonX < -0.999f){
            centers[2*i] = -0.999f+radiusArray[i];
                speeds[2*i] = -0.7f*speeds[2*i];
        }

        if(topX > 0.999f){
            centers[2*i] = 0.999f - radiusArray[i];
            speeds[2*i] = -0.7f*speeds[2*i];        
        }
    }
}

float getDistance(std::vector<float>& centers, int circle1, int circle2){
    float dx = centers[2*circle2] - centers[2*circle1];
    float dy = centers[2*circle2+1] - centers[2*circle1+1];
    float distance = std::sqrt( pow(dx,2) + pow(dy,2) );

    return distance-radiusArray[circle1]-radiusArray[circle2];
}

void ballCollisions(std::vector<float>& centers, int numCircles, std::vector<float>& speeds){
    for(int i=0; i<numCircles; i++){
        for(int j=i+1; j<numCircles; j++){
            if(checkCollision(centers, i, j)){
                resolveCollision(centers, i, j, speeds);
            }
        }
    }
}

bool checkCollision(std::vector<float>& centers, int index1, int index2){
    float distance = getDistance(centers, index1, index2);

    if(distance <= 0.0f){
        return true;
    }
    return false;
}

void resolveCollision(std::vector<float>& centers, int index1, int index2, std::vector<float>& speeds){
    speeds[2*index1] *= -0.7f;
    speeds[2*index1+1] *= -0.7f;
    speeds[2*index2] *= -0.7f;
    speeds[2*index2+1] *= -0.7f;
    
    centers[2*index1] += 0.1f;
    centers[2*index1+1] += 0.1f;
    centers[2*index2] -= 0.1f;
    centers[2*index2+1] -= 0.1f;
}
