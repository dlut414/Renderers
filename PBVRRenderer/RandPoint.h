/*LICENSE*/
#ifndef RANDPOINT_H
#define RANDPOINT_H

#include <cstdlib>
#include <cmath>
class RandPoint
{
public:
    RandPoint(const unsigned& _seed = 0) : seed(_seed)
    {
        srand(seed);
    }
    ~RandPoint()
    {}

    inline const float roll() const
    {
        return genRandFloat();
    }

    inline const Point rollPoint() const
    {
        return Point(genRandFloat(), genRandFloat(), genRandFloat());
    }

    inline const Point rollPoint(const BBox& box) const
    {
        return Point( genRandFloat(box.pMin.x, box.pMax.x),
                      genRandFloat(box.pMin.y, box.pMax.y),
                      genRandFloat(box.pMin.z, box.pMax.z) );
    }

    inline const Point rollPoint(const float& _r) const
    {
        /*
        float r     = genRandFloat(0.f, _r);
        float phi   = genRandFloat(0.f, M_PI);
        float theta = genRandFloat(0.f, 2*M_PI);

        return Point(r*sin(phi)*cos(theta), r*sin(phi)*sin(theta), r*cos(phi));
        */
        Point tmp = Point( genRandFloat(-1.f, 1.f), genRandFloat(-1.f, 1.f), genRandFloat(-1.f, 1.f) );
        while( (tmp - Point(0.f, 0.f, 0.f)).mag2() > 1.f )
        {
            tmp = Point( genRandFloat(-1.f, 1.f), genRandFloat(-1.f, 1.f), genRandFloat(-1.f, 1.f) );
        }
        return _r*tmp;
    }

    inline const Point rollPoint(const Point& _p, const float& _r) const
    {
        Point tmp = rollPoint(_r);
        return Point(tmp.x + _p.x, tmp.y + _p.y, tmp.z + _p.z);
    }

private:
    inline float genRandFloat() const
    {
        return ( static_cast<float>(rand()) / static_cast<float>(RAND_MAX) );
    }

    inline float genRandFloat(const float& lo, const float& hi) const
    {
        //return ( lo + (hi - lo) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) );
        return static_cast<float>( lo + (hi - lo) * genRandFloat() );
    }

private:
    unsigned seed;
};

#endif // RANDPOINT_H
