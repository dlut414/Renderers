/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Published under CC BY-NC
*/
#ifndef POINT_H
#define POINT_H

#include "Vector.h"

class Point
{
public:
    Point(const float& _x = 0, const float& _y = 0, const float& _z = 0) : x(_x), y(_y), z(_z)
    {}
    ~Point()
    {}

    Vector operator- (const Point& p) const
    {
        return Vector(x-p.x, y-p.y, z-p.z);
    }

    void operator+= (const Vector& v)
    {
        x += v.x;   y += v.y;   z += v.z;
    }
    void operator-= (const Vector& v)
    {
        x -= v.x;   y -= v.y;   z -= v.z;
    }

public:
    float x, y, z;

};

Point operator* (const float& s, const Point& p)
{
    return Point(s*p.x, s*p.y, s*p.z);
}
#endif // POINT_H
