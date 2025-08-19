#include "gravity.h"

std::vector<float> center;

void isEdge(std::vector<float>& centers, int numCircles, std::vector<float>& speeds, float xSquare){
    for(int i=0; i<numCircles; i++){
        int idx = 2*i;
        float bottonY = centers[idx+1]-radius;
        float bottonX = centers[idx]-radius;
        float topY = centers[idx+1]+radius;
        float topX = centers[idx]+radius;
        
        if(bottonY<-1.0f){
            centers[2*i+1] = -1.0f + radius;
            speeds[idx+1] *= -0.25f;
        }else if(topY>1.0f){
            centers[idx+1] = 1.0f-radius;
            speeds[idx+1] = -0.25f*speeds[idx+1];
        }

        if(bottonX < -1.0f){
            centers[idx] = -1.0f + radius;
            speeds[idx] = -0.25f*speeds[idx];
        }else if(topX>xSquare){
            centers[idx] = xSquare-radius;
            speeds[idx] = -0.25f*speeds[idx];
        }
    }
}

float getDistance(std::vector<float>& centers, int circle1, int circle2){
    float dx = centers[2*circle2] - centers[2*circle1];
    float dy = centers[2*circle2+1] - centers[2*circle1+1];
    float distance = std::sqrt( pow(dx,2) + pow(dy,2) );

    return distance;
}
