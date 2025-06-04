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
