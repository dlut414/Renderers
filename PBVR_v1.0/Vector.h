/*LICENSE*/
#ifndef VECTOR_H
#define VECTOR_H


class Vector
{
public:
    Vector(const float& _x = 0, const float& _y = 0, const float& _z = 0) : x(_x), y(_y), z(_z)
    {}
    ~Vector()
    {}

    Vector operator* (const float& s) const
    {
        return Vector(s*x, s*y, s*z);
    }

    void operator+= (const Vector& v)
    {
        x+=v.x; y+=v.y; z+=v.z;
    }

public:
    float x, y, z;

};

Vector operator* (const float& s, const Vector& v)
{
    return Vector(s*v.x, s*v.y, s*v.z);
}

Vector operator+ (const Vector& v1, const Vector& v2)
{
    return Vector(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

#endif // VECTOR_H
