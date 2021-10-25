#ifndef QUATERNION_H
#define QUATERNION_H
#include "tools.h"

class Quaternion
{
public:
    Quaternion();
    GETSET(float, x)
    GETSET(float, y)
    GETSET(float, z)
    GETSET(float, w)
    Quaternion(float _x, float _y, float _z, float _w);
    void Normalize();
    Quaternion Conjugate();
};

#endif // QUATERNION_H
