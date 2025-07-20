#include "gravity.h"

std::vector<float> center;

void gravity(std::vector<float>& centers, int numCircles, std::vector<float>& speeds, float timeDiff){
    float g = -3.5f;
    float ax = 0.99f;
    for(int i=0; i<numCircles; i++){
        speeds[2*i+1] += g*timeDiff;

        centers[2*i+1] += speeds[2*i+1]*timeDiff;

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
        float bottonY = centers[2*i+1]-radius;
        float bottonX = centers[2*i]-radius;
        float topY = centers[2*i+1]+radius;
        float topX = centers[2*i]+radius;
        
        if(bottonY<-0.999f){
            centers[2*i+1] = -0.999f + radius;
            if(std::abs(speeds[2*i+1]) < 0.25f){
                speeds[2*i+1] = 0.0f;
            }else{
                speeds[2*i+1] = -0.55f * speeds[2*i+1];
            }
        }

        if(topY > 0.999f){
            centers[2*i+1] = 0.999f - radius;
            speeds[2*i+1] = -0.55f*speeds[2*i+1];
        }

        if(bottonX < -0.999f){
            centers[2*i] = -0.999f+radius;
            speeds[2*i] = -0.7f*speeds[2*i];
        }

        if(topX > 0.999f){
            centers[2*i] = 0.999f - radius;
            speeds[2*i] = -0.7f*speeds[2*i];        
        }
    }
}

float getDistance(std::vector<float>& centers, int circle1, int circle2){
    float dx = centers[2*circle2] - centers[2*circle1];
    float dy = centers[2*circle2+1] - centers[2*circle1+1];
    float distance = std::sqrt( pow(dx,2) + pow(dy,2) );

    return distance-radius*2;
}

void ballCollisions(std::vector<float>& centers, int numCircles, std::vector<float>& speeds){
    for(int i=0; i<numCircles; i++){
        for(int j=i+1; j<numCircles; j++){
            if(checkCollision(centers, i, j)){

                float vX = speeds[2*i]-speeds[2*j];
                float vY = speeds[2*i+1]-speeds[2*j+1];
                float v = std::sqrt(vX*vX + vY*vY);

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
    float vx1 = speeds[2*index1];
    float vx2 = speeds[2*index2];
    float vy1 = speeds[2*index1+1];
    float vy2 = speeds[2*index2+1];

    float dx = centers[2*index2] - centers[2*index1];
    float dy = centers[2*index2+1] - centers[2*index1+1];

    float dist = std::sqrt(dx*dx + dy*dy);
    
    if(dist == 0.0f){
        dist = 0.000001f;
    }

    float nx = dx/dist;
    float ny = dy/dist;

    float proyection = (vx1-vx2) * nx + (vy1-vy2) * ny;
    float restitution = 0.3f;
    float j = -(1+restitution)*proyection/2.0f;

    speeds[2*index1] += j*nx;
    speeds[2*index2] -= j*nx;
    speeds[2*index1+1] += j*ny;
    speeds[2*index2+1] -= j*ny;

    float overlap = radius*2-dist;

    if(overlap > 0){
        float correction = overlap/2.0f;
        centers[2*index1] -= nx * correction;
        centers[2*index1+1] -= ny * correction;
        centers[2*index2] += nx * correction;
        centers[2*index2+1] += ny * correction;
    }
}
