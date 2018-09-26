/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Published under CC BY-NC
*/
#ifndef PARTTOCUBE_H
#define PARTTOCUBE_H

#include <cmath>
#include "common.h"
#include "Voxel.h"
#include "PartTo_.h"

class PartToCube : public PartTo_
{
public:
    PartToCube(const int& _type = 0, const float& _voxWidth = 1.f, const float& _meshWidth = 1.f)
    : PartTo_(_type, _voxWidth), meshWidth(_meshWidth)
    {
        meshWidthInv    = (_meshWidth==0.f) ? (0.f) : (1.f / _meshWidth);
    }
    ~PartToCube() {}

    void build()
    {
        PartTo_::buildVoxel  ();
        buildMesh   ();
    }

    float capacity() const
    {
        return meshWidth * meshWidth * meshWidth;
    }

    BBox bound(const unsigned& id) const
    {
        unsigned _z = id / (i_cx * i_cy);
        unsigned _y = ( id - (_z * (i_cx * i_cy)) ) / i_cx;
        unsigned _x = id - (_z * (i_cx * i_cy)) - _y * i_cx;

        float _fx = PartTo_::box.pMin.x + _x * meshWidth;
        float _fy = PartTo_::box.pMin.y + _y * meshWidth;
        float _fz = PartTo_::box.pMin.z + _z * meshWidth;

        return BBox(Point(_fx, _fy, _fz), Point(_fx+meshWidth, _fy+meshWidth, _fz+meshWidth));
    }
    BBox bound(const unsigned& _idx, const unsigned& _idy, const unsigned& _idz) const
    {
        float _fx = PartTo_::box.pMin.x + _idx * meshWidth;
        float _fy = PartTo_::box.pMin.y + _idy * meshWidth;
        float _fz = PartTo_::box.pMin.z + _idz * meshWidth;
        return BBox(Point(_fx, _fy, _fz), Point(_fx+meshWidth, _fy+meshWidth, _fz+meshWidth));
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

        Vector _v = Vector( (p - vert[_id[0]]) * meshWidthInv );

        _ret += 0.125f * (1 - _v.x) * (1 - _v.y) * (1 - _v.z) * scalar[_id[0]];
        _ret += 0.125f * (1 + _v.x) * (1 - _v.y) * (1 - _v.z) * scalar[_id[1]];
        _ret += 0.125f * (1 + _v.x) * (1 + _v.y) * (1 - _v.z) * scalar[_id[2]];
        _ret += 0.125f * (1 - _v.x) * (1 + _v.y) * (1 - _v.z) * scalar[_id[3]];
        _ret += 0.125f * (1 - _v.x) * (1 - _v.y) * (1 + _v.z) * scalar[_id[4]];
        _ret += 0.125f * (1 + _v.x) * (1 - _v.y) * (1 + _v.z) * scalar[_id[5]];
        _ret += 0.125f * (1 + _v.x) * (1 + _v.y) * (1 + _v.z) * scalar[_id[6]];
        _ret += 0.125f * (1 - _v.x) * (1 + _v.y) * (1 + _v.z) * scalar[_id[7]];

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

        Vector _v = Vector( (p - vert[_id[0]]) * meshWidthInv );

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
    unsigned    numCube;
    unsigned    numVert;
    unsigned    i_cx, i_cy, i_cz;
    unsigned    i_vx, i_vy, i_vz;

    float       meshWidth, meshWidthInv;

    std::vector<float>  scalar;
    std::vector<Point>  vert;
    std::vector<Vector> norm;

private:
    inline unsigned offset_cube(const Point& p) const
    {
        unsigned _x = (p.x - PartTo_::box.pMin.x) * meshWidthInv;
        unsigned _y = (p.y - PartTo_::box.pMin.y) * meshWidthInv;
        unsigned _z = (p.z - PartTo_::box.pMin.z) * meshWidthInv;

        return (_z * i_cx * i_cy) + (_y * i_cx) + _x;
    }
    inline unsigned offset_vert(const Point& p) const
    {
        unsigned _x = (p.x - PartTo_::box.pMin.x) * meshWidthInv;
        unsigned _y = (p.y - PartTo_::box.pMin.y) * meshWidthInv;
        unsigned _z = (p.z - PartTo_::box.pMin.z) * meshWidthInv;

        return (_z * i_vx * i_vy) + (_y * i_vx) + _x;
    }
    inline unsigned offset_vert(const unsigned& id) const
    {
        unsigned _z = id / (i_cx * i_cy);
        unsigned _y = ( id - (_z * (i_cx * i_cy)) ) / i_cx;
        unsigned _x = id - (_z * (i_cx * i_cy)) - (_y * i_cx);

        float _fx = PartTo_::box.pMin.x + _x * meshWidth + Common::EPS;
        float _fy = PartTo_::box.pMin.y + _y * meshWidth + Common::EPS;
        float _fz = PartTo_::box.pMin.z + _z * meshWidth + Common::EPS;

        return offset_vert( Point(_fx, _fy, _fz) );
    }

    inline float weight_cubic(const float& _r0, const float& _r)
    {
        float __q = _r / _r0;

        if(__q <= 0.5f) return 8.0f * Common::OVERPI * (1 - 6*__q*__q + 6*__q*__q*__q);
        else if(__q <= 1.0f) return 16.0f * Common::OVERPI * (1.0f-__q) * (1.0f-__q) * (1.0f-__q);
        else return 0.0f;
    }

    void buildMesh()
    {
        scalar.     clear();
        vert.       clear();
        norm.       clear();

        i_cx = floor( (PartTo_::box.pMax.x - box.pMin.x) * meshWidthInv );
        i_cy = floor( (PartTo_::box.pMax.y - box.pMin.y) * meshWidthInv );
        i_cz = floor( (PartTo_::box.pMax.z - box.pMin.z) * meshWidthInv );

        if(i_cx == 0) i_cx++;
        if(i_cy == 0) i_cy++;
        if(i_cz == 0) i_cz++;

        i_vx = i_cx+1;  i_vy = i_cy+1;  i_vz = i_cz+1;

        numCube = i_cx * i_cy * i_cz;
        numVert = i_vx * i_vy * i_vz;

        for(unsigned k=0; k<i_vz; k++)
        {
            for(unsigned j=0; j<i_vy; j++)
            {
                for(unsigned i=0; i<i_vx; i++)
                {
                    vert.       push_back( Point(PartTo_::box.pMin.x + i*meshWidth,
                                                 PartTo_::box.pMin.y + j*meshWidth,
                                                 PartTo_::box.pMin.z + k*meshWidth) );
                    norm.       push_back( Vector(0.f, 0.f, 0.f) );
                    scalar.     push_back( 0.f );
                }
            }
        }

        for(unsigned i=0; i<scalar.size(); i++)
        {
            Point       _verti  = vert[i];
            unsigned    _vox    = offset_voxel(_verti);

            for(unsigned nb = 0; nb < PartTo_::voxObj->neiList[_vox].size(); nb++)
            {
                unsigned _nbid = PartTo_::voxObj ->  neiList[_vox][nb];

                for(unsigned j = 0; j < PartTo_::voxObj->eleList[_nbid].size(); j++)
                {
                    unsigned _partjId = PartTo_::voxObj  ->  eleList[_nbid][j];

                    Point __partj   =   PartTo_::part[_partjId];
                    float __dr      =   sqrt( (__partj - _verti).mag2() );
                    scalar[i]       +=  weight_cubic(voxWidth, __dr);
                }
            }
            /*
            for(unsigned j=0; j<part.size(); j++)
            {
                Point __partj   = part[j];
                float __rr      = (__partj - _verti).mag2();
                scalar[i] += weight_cubic(voxWidth, sqrt(__rr));
            }
            */
        }

        for(unsigned i=0; i<norm.size(); i++)
        {
            if( i + i_vx*i_vy < numVert && i >= i_vx*i_vy )
            {
                float _n[6];
                _n[0] = scalar[i-1];
                _n[1] = scalar[i+1];
                _n[2] = scalar[i-i_vx];
                _n[3] = scalar[i+i_vx];
                _n[4] = scalar[i-i_vx*i_vy];
                _n[5] = scalar[i+i_vx*i_vy];

                ///out pointing
                norm[i].x = _n[0] - _n[1];
                norm[i].y = _n[2] - _n[3];
                norm[i].z = _n[4] - _n[5];
            }
        }
    }

};

#endif // PARTTOCUBE_H
