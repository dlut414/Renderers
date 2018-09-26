/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Released under CC BY-NC
*/
#ifndef CUBE_H
#define CUBE_H

#include <cmath>
#include <vector>
#include "../common/typedef/Point.h"
#include "../common/typedef/Vector.h"
#include "../common/typedef/BBox.h"
#include "Volume.h"

class Cube : public Volume
{
public:
    Cube(const BBox& _box = BBox(), const float& _width = 1.f) : Volume(_box), width(_width)
    {
        widthInv = (_width == 0) ? 0.f : (1.f / _width);

        buildBBox();

        allocVert();
    }
    ~Cube()
    {
        freeVert();
    }

    bool build()
    {
        buildBBox();

        if(numVert > vert.max_size() || numVert > norm.max_size() || numVert > alpha.max_size()) return 0;

        vert.   clear();
        norm.   clear();
        alpha.  clear();

        for(unsigned k=0; k<i_vz; k++)
        {
            for(unsigned j=0; j<i_vy; j++)
            {
                for(unsigned i=0; i<i_vx; i++)
                {
                    vert.   push_back( Point(Volume::box.pMin.x + i*width,
                                             Volume::box.pMin.y + j*width,
                                             Volume::box.pMin.z + k*width) );

                    norm.   push_back( Vector(0.f, 0.f, 0.f) );

                    alpha.  push_back( 0.f );
                }
            }
        }
        return 1;
    }

    inline float capacity() const
    {
        return width * width * width;
    }

    inline unsigned offset_cube(const Point& p) const
    {
        unsigned _x = (p.x - Volume::box.pMin.x) * widthInv;
        unsigned _y = (p.y - Volume::box.pMin.y) * widthInv;
        unsigned _z = (p.z - Volume::box.pMin.z) * widthInv;

        return (_z * i_cx * i_cy) + (_y * i_cx) + _x;
    }

    inline unsigned offset_vert(const Point& p) const
    {
        unsigned _x = (p.x - Volume::box.pMin.x) * widthInv;
        unsigned _y = (p.y - Volume::box.pMin.y) * widthInv;
        unsigned _z = (p.z - Volume::box.pMin.z) * widthInv;

        return (_z * i_vx * i_vy) + (_y * i_vx) + _x;
    }

    inline BBox bound(const unsigned& id) const
    {
        unsigned _z = id / (i_cx * i_cy);
        unsigned _y = ( id - (_z * (i_cx * i_cy)) ) / i_cx;
        unsigned _x = id - (_z * (i_cx * i_cy)) - _y * i_cx;

        float _fx = Volume::box.pMin.x + _x * width;
        float _fy = Volume::box.pMin.y + _y * width;
        float _fz = Volume::box.pMin.z + _z * width;

        return BBox(Point(_fx, _fy, _fz), Point(_fx+width, _fy+width, _fz+width));
    }
    inline BBox bound(const unsigned& _idx, const unsigned& _idy, const unsigned& _idz) const
    {
        float _fx = Volume::box.pMin.x + _idx * width;
        float _fy = Volume::box.pMin.y + _idy * width;
        float _fz = Volume::box.pMin.z + _idz * width;
        return BBox(Point(_fx, _fy, _fz), Point(_fx+width, _fy+width, _fz+width));
    }

    float inpAlpha(const Point& p) const
    {
        float _ret = 0.f;

        unsigned _id[8];
        _id[0] = offset_vert(p);
        _id[1] = _id[0] + 1;
        _id[2] = _id[1] + i_vx;
        _id[3] = _id[2] - 1;
        _id[4] = _id[0] + i_vx * i_vy;
        _id[5] = _id[4] + 1;
        _id[6] = _id[5] + i_vx;
        _id[7] = _id[6] - 1;

        Vector _v = Vector( (p - vert[_id[0]]) * widthInv );

        _ret += 0.125f * (1 - _v.x) * (1 - _v.y) * (1 - _v.z) * alpha[_id[0]];
        _ret += 0.125f * (1 + _v.x) * (1 - _v.y) * (1 - _v.z) * alpha[_id[1]];
        _ret += 0.125f * (1 + _v.x) * (1 + _v.y) * (1 - _v.z) * alpha[_id[2]];
        _ret += 0.125f * (1 - _v.x) * (1 + _v.y) * (1 - _v.z) * alpha[_id[3]];
        _ret += 0.125f * (1 - _v.x) * (1 - _v.y) * (1 + _v.z) * alpha[_id[4]];
        _ret += 0.125f * (1 + _v.x) * (1 - _v.y) * (1 + _v.z) * alpha[_id[5]];
        _ret += 0.125f * (1 + _v.x) * (1 + _v.y) * (1 + _v.z) * alpha[_id[6]];
        _ret += 0.125f * (1 - _v.x) * (1 + _v.y) * (1 + _v.z) * alpha[_id[7]];

        return _ret;
    }

    Vector inpNormal(const Point& p) const
    {
        Vector _ret = Vector(0.f, 0.f, 0.f);

        unsigned _id[8];
        _id[0] = offset_vert(p);
        _id[1] = _id[0] + 1;
        _id[2] = _id[1] + i_vx;
        _id[3] = _id[2] - 1;
        _id[4] = _id[0] + i_vx * i_vy;
        _id[5] = _id[4] + 1;
        _id[6] = _id[5] + i_vx;
        _id[7] = _id[6] - 1;

        Vector _v = Vector( (p - vert[_id[0]]) * widthInv );

        _ret += 0.125f * (1 - _v.x) * (1 - _v.y) * (1 - _v.z) * norm[_id[0]];
        _ret += 0.125f * (1 + _v.x) * (1 - _v.y) * (1 - _v.z) * norm[_id[1]];
        _ret += 0.125f * (1 + _v.x) * (1 + _v.y) * (1 - _v.z) * norm[_id[2]];
        _ret += 0.125f * (1 - _v.x) * (1 + _v.y) * (1 - _v.z) * norm[_id[3]];
        _ret += 0.125f * (1 - _v.x) * (1 - _v.y) * (1 + _v.z) * norm[_id[4]];
        _ret += 0.125f * (1 + _v.x) * (1 - _v.y) * (1 + _v.z) * norm[_id[5]];
        _ret += 0.125f * (1 + _v.x) * (1 + _v.y) * (1 + _v.z) * norm[_id[6]];
        _ret += 0.125f * (1 - _v.x) * (1 + _v.y) * (1 + _v.z) * norm[_id[7]];

        return _ret;
    }

public:
    float width, widthInv;
    unsigned i_cx, i_cy, i_cz;
    unsigned i_vx, i_vy, i_vz;
    unsigned numCube;
    unsigned numVert;

    std::vector<Point>  vert;
    std::vector<float>  alpha;
    std::vector<Vector> norm;

private:
    void buildBBox()
    {
        i_cx = floor( (Volume::box.pMax.x - Volume::box.pMin.x) * widthInv );
        i_cy = floor( (Volume::box.pMax.y - Volume::box.pMin.y) * widthInv );
        i_cz = floor( (Volume::box.pMax.z - Volume::box.pMin.z) * widthInv );

        if(i_cx == 0) i_cx++;
        if(i_cy == 0) i_cy++;
        if(i_cz == 0) i_cz++;

        i_vx = i_cx+1;  i_vy = i_cy+1;  i_vz = i_cz+1;

        numCube = i_cx * i_cy * i_cz;
        numVert = i_vx * i_vy * i_vz;
    }
    void allocVert()
    {
    }
    void freeVert()
    {
    }

};

#endif // CUBE_H
