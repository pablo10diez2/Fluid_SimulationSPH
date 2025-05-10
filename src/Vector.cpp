#include "Vector.h"
#include <iostream>

using namespace std;

Vector::Vector(int x, int y, int z){
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vector::setX(int x){
    this->x=x;
}

void Vector::setY(int y){
    this->y=y;
}

void Vector::setZ(int z){
    this->z=z;
}

int Vector::getX(){
    return this->x;
}

int Vector::getY(){
    return this->y;
}

int Vector::getZ(){
    return this->z;
}

void Vector::print(){
    cout <<"x:"<<x <<" y:"<<y << " z:"<<z; 
}
