/*LICENSE*/
#ifndef PARTGEN_H
#define PARTGEN_H

#include <assert.h>
#include <cmath>
#include <vector>
#include "include/common.h"
#include "Cube.h"
#include "RandPoint.h"

class PartGen
{
public:
    PartGen(const float& r = 0.f, const float& t = 1.f, const unsigned& s = 0)
        : radius(r), dt(t), rp(s)
    {
        allocPart();
    }
    ~PartGen()
    {
        freePart();
    }

    void genPart_MP(const Cube* const obj)
    {
/*
        unsigned _numPart   = 0;
        unsigned _id        = 0;
*/
        for(unsigned level=0; level<REPEAT_LEVEL; level++)
        {
            rp = RandPoint(level);

            for(unsigned k=0; k<(obj->i_z); k++)
            {
                for(unsigned j=0; j<(obj->i_y); j++)
                {
                    for(unsigned i=0; i<(obj->i_x); i++)
                    {
                        BBox        __box           = obj->bound(i, j, k);
                        Point       __pt1           = rp.rollPoint(__box);
                        float       __dens1         = rho(obj->inpAlpha(__pt1));
                        unsigned    __partInCube    = rho(obj->inpAlpha(__box.pCenter())) * (obj->capacity());

                        if(__partInCube == 0) continue;
/*
                        _numPart += __partInCube;

                        part    [_id] = __pt1;
                        normal  [_id] = obj->inpNormal(__pt1);
                        _id++;
*/
                        part.   push_back( __pt1 );
                        normal. push_back( obj->inpNormal(__pt1) );

                        for(unsigned n=1; n<__partInCube; n++)
                        {
                            Point __pt2     = rp.rollPoint(obj->bound(i, j, k));
                            float __dens2   = rho(obj->inpAlpha(__pt2));
                            float __w       = __dens2 / __dens1;

                            if(__w > 1.f || rp.roll() <= __w)
                            {
                                __pt1   = __pt2;
                                __dens1 = __dens2;
                            }
/*
                            part    [_id] = __pt1;
                            normal  [_id] = obj->inpNormal(__pt1);
                            _id++;
*/
                            part.   push_back( __pt1 );
                            normal. push_back( obj->inpNormal(__pt1) );
                        }
                    }
                }
            }
/*
            assert(_numPart <= numPart);
*/
        }
/*
        numPart = _numPart;
*/
        numPart = part.size();
    }

public:
    unsigned    numPart;
    float       radius;
    float       dt;
/*
    Point*      part;
    Vector*     normal;
*/
    std::vector<Point>  part;
    std::vector<Vector> normal;

private:
    void allocPart()
    {
        /*
        part    = new Point [numPart];
        normal  = new Vector[numPart];
        */
    }
    void freePart()
    {
        /*
        delete[] part;      part    = NULL;
        delete[] normal;    normal  = NULL;
        */
        part.~vector();
        normal.~vector();
    }

    float rho(const float& alpha) const
    {
        float _rho = 1.f / (8.f * radius * radius * radius);
        float _alphaMax = 1.f - exp(-mytype::PI * radius * radius * _rho * dt);

        if(alpha > _alphaMax) return _rho;
        else return -log(1.f - alpha) / (mytype::PI * radius * radius * dt);
    }

private:
    RandPoint rp;

};

#endif // PARTGEN_H
