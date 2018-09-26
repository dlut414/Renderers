/*
LICENCE
*/
//typedef.h
//file to define types
#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <sys/timeb.h>

namespace mytype
{

    typedef enum
    {
        x,y,z
    }DIR;

    typedef unsigned Integer;
    typedef float    Real;

    inline long getSystemTime() {
        timeb t;
        ftime(&t);
        return (1000 * t.time + t.millitm);
    }

    typedef struct
    {
        Integer x;
        Integer y;
        Integer z;
    }Int3;

    typedef struct
    {
        Real x;
        Real y;
        Real z;
    }Real3;

    inline Real3 operator+ (const Real3& a , const Real3& b)
    {
        Real3 c;
        c.x = a.x + b.x;
        c.y = a.y + b.y;
        c.z = a.z + b.z;
        return c;
    }

    inline Real operator* (const Real3& a , const Real3& b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    inline Real3 operator* (const Real& a , const Real3& b)
    {
        Real3 c;
        c.x = a * b.x;
        c.y = a * b.y;
        c.z = a * b.z;
        return c;
    }

    inline Real3 operator- (const Real3& a , const Real3& b)
    {
        Real3 c;
        c.x = a.x - b.x;
        c.y = a.y - b.y;
        c.z = a.z - b.z;
        return c;
    }

    inline void operator+= (Real3& a , const Real3& b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
    }

    inline Integer ROUND(Real x)
    {
        return Integer(x+0.5f);
    }

}

#endif //TYPEDEF_H
