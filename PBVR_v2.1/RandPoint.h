/*LICENSE*/
#ifndef RANDPOINT_H
#define RANDPOINT_H

#include <cstdlib>
class RandPoint
{
public:
    RandPoint(const unsigned& _seed = 0) : seed(_seed)
    {
        srand(seed);
    }
    ~RandPoint()
    {}

    inline float roll() const
    {
        return genRandFloat();
    }

    Point rollPoint() const
    {
        return Point(genRandFloat(), genRandFloat(), genRandFloat());
    }

    Point rollPoint(const BBox& box) const
    {
        return Point( genRandFloat(box.pMin.x, box.pMax.x),
                      genRandFloat(box.pMin.y, box.pMax.y),
                      genRandFloat(box.pMin.z, box.pMax.z) );
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
