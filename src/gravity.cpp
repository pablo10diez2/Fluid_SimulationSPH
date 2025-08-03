#include "gravity.h"

std::vector<float> center;

void isEdge(std::vector<float>& centers, int numCircles, std::vector<float>& speeds){
    for(int i=0; i<numCircles; i++){
        float bottonY = centers[2*i+1]-radius;
        float bottonX = centers[2*i]-radius;
        float topY = centers[2*i+1]+radius;
        float topX = centers[2*i]+radius;
        
        if(bottonY<-1.0f){
            centers[2*i+1] = -1.0f + radius;
            if(std::abs(speeds[2*i+1]) < 0.25f){
                speeds[2*i+1] = 0.0f;
            }else{
                speeds[2*i+1] = -0.55* speeds[2*i+1];
            }
        }

        if(topY > 1.0f){
            centers[2*i+1] = 0.999f - radius;
            speeds[2*i+1] = -0.55f*speeds[2*i+1];
        }

        if(bottonX < -1.0f){
            centers[2*i] = -0.999f+radius;
            speeds[2*i] = -0.7f*speeds[2*i];
        }

        if(topX > 1.0f){
            centers[2*i] = 0.999f - radius;
            speeds[2*i] = -0.7f*speeds[2*i];        
        }
    }
}

float getDistance(std::vector<float>& centers, int circle1, int circle2){
    float dx = centers[2*circle2] - centers[2*circle1];
    float dy = centers[2*circle2+1] - centers[2*circle1+1];
    float distance = std::sqrt( pow(dx,2) + pow(dy,2) );

    return distance;
}
