#include "gravity.h"

void gravity(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds){
    for(int i=0; i<numCircles; i++){
        if(speeds[i] != 0.0f){
            float acc = 0.0001f;
            float calculo = speeds[i] -acc;
            speeds[i] = calculo;

            for(int z=0; z<numTriangles*3*6; z+=6){
                vertices[z+1+(i*numTriangles*3*6)]=vertices[z+1+(i*numTriangles*3*6)]+speeds[i];
            }
        }
    }
}

void isEdge(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds){
    for(int i =0; i<numCircles; i++){
        for(int z=0; z<numTriangles*3*6; z+=6){
            int index = z+1+(i*numTriangles*3*6);
                
            if(vertices[index]<= -0.9999f){
                speeds[i] = 0.58*std::abs(speeds[i]);
                if(std::abs(speeds[i]<0.0001f)){
                    speeds[i] = 0.0f;
                }break;
            }
        }
    }
}

float* getDistance(int circle1, int circle2){
    float* distance= new float[2];

    distance[0] = abs( (centers[circle1]-radiusArray[circle1]) + (centers[circle2]+radiusArray[circle2]));
    distance[1] = abs( (centers[circle1+1]-radiusArray[circle1]) + (centers[circle2+1]+radiusArray[circle2]));
    return distance;
}

void ballCollisions(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds){
    for(int i = 0; i<numCircles; i++){
        for(int z= 0; z<numCircles; z++){
            if(i!=z){
                float* distance = getDistance(i, z);
                if(distance[0] <= 0.001f && distance[1] <= 0.001f){
                    std::cout<<"Collision: "<<i<<"-"<<z<<"Distance: "<<distance[0]<<"-"<<distance[1]<<std::endl;
                }
                delete[] distance;
            }   
        }
    }
}
