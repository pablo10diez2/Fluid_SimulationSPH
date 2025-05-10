#ifndef VECTOR_H_
#define VECTOR_H_

class Vector{
private:
    int x,y,z;
public:
    Vector(int x, int y, int z);
    void setX(int x);
    void setY(int y);
    void setZ(int z);
    int getX();
    int getY();
    int getZ();
    void print();

};
#endif
