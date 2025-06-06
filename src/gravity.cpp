#include "gravity.h"

std::vector<float> center;

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

void getCenters(std::vector<float>& vertices, int numTriangles, int numCircles, int circle){
    int index = circle*numTriangles*3*6;
    float xCenter = vertices[index]-radiusArray[circle];
    float yCenter = vertices[index+1];

    center.clear();
    std::cout<<"clear arriba"<<std::endl;
    center.push_back(xCenter);
    center.push_back(yCenter);
}

float* getDistance(std::vector<float>& vertices, int numTriangles, int numCircles, int circle1, int circle2){
    getCenters(vertices, numTriangles, numCircles, circle1);
    float center1x = center[0];
    float center1y = center[1];

    getCenters(vertices, numTriangles, numCircles, circle2);
    float center2x = center[0];
    float center2y = center[1];
    
    std::cout<<"center1x "<<center1x<<std::endl;
    std::cout<<"center1y "<<center1y<<std::endl;
    std::cout<<"center2x "<<center2x<<std::endl;
    std::cout<<"center2y "<<center2y<<std::endl;

    float* distance= new float[2];
    distance[0] = abs( (center1x-radiusArray[circle1]) + (center2x+radiusArray[circle2]));
    distance[1] = abs( (center1y-radiusArray[circle1]) + (center2y+radiusArray[circle2]));

    std::cout<<distance[0]<<std::endl;
    std::cout<<distance[1]<<std::endl;
    return distance;
}

void ballCollisions(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds){
    for(int i = 0; i<numCircles; i++){
        for(int z= 0; z<numCircles; z++){
            if(i!=z){
                float* distance = getDistance(vertices, numTriangles, numCircles, i, z);
                if((distance[0] <= 0.001f) && (distance[1] <= 0.001f)){
                    std::cout<<"Collision: "<<i<<"-"<<z<<"Distance: "<<distance[0]<<"-"<<distance[1]<<std::endl;
                }
                delete[] distance;
            }   
        }
    }
}
