/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Published under CC BY-NC
*/
#ifndef PARTTOTRI_H
#define PARTTOTRI_H

#include <cmath>
#include "common.h"
#include "header.h"
#include "Voxel.h"
#include "PartTo_.h"
#include "mc_table.h"

class PartToTri : public PartTo_
{
public:
    PartToTri(const int& _type = 0, const float& _voxWidth = 1.f, const float& _beta = 0.f, const float& _meshWidth = 1.f)
    : PartTo_(_type, _voxWidth), beta(_beta), meshWidth(_meshWidth)
    {
        meshWidthInv    = (_meshWidth==0.f) ? (0.f) : (1.f / _meshWidth);
    }
    ~PartToTri() {}

    void build()
    {
        PartTo_::buildVoxel  ();
        buildMesh   ();
        buildTri    ();
    }

public:
    float       beta;
    float       isoValue;
    float       meshWidth, meshWidthInv;

    std::vector<float>  scalar;
    std::vector<Point>  vert;
    std::vector<Vector> vertNorm;
    std::vector<Point>  tri;
    std::vector<Vector> triNorm;

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
    inline void vertInterp(Point& edge, const Point& v1, const Point& v2, const float& n1, const float& n2, const float& iso) const
    {
        const float _dnInv = 1.f / (n2 - n1);

        ///v == ( n(v2-v1) + n2v1 - n1v2 ) / (n2 - n1)
        edge.x = (iso * (v2.x - v1.x) + n2 * v1.x - n1 * v2.x ) * _dnInv;
        edge.y = (iso * (v2.y - v1.y) + n2 * v1.y - n1 * v2.y ) * _dnInv;
        edge.z = (iso * (v2.z - v1.z) + n2 * v1.z - n1 * v2.z ) * _dnInv;
    }
    inline void vertInterp0(Point& edge, const Point& v1, const Point& v2, const float& n1, const float& n2) const
    {
        const float _dnInv = 1.f / (n2 - n1);

        ///v(n=0) == (n2v1 - n1v2) / (n2 - n1)
        edge.x = (n2 * v1.x - n1 * v2.x ) * _dnInv;
        edge.y = (n2 * v1.y - n1 * v2.y ) * _dnInv;
        edge.z = (n2 * v1.z - n1 * v2.z ) * _dnInv;
    }
    inline void vertInterp2(Point& edge, const Point& v1, const Point& v2) const
    {
        edge.x = 0.5f * (v1.x + v2.x);
        edge.y = 0.5f * (v1.y + v2.y);
        edge.z = 0.5f * (v1.z + v2.z);
    }
    inline void normInterp(Vector& norm, const float& v01, const float& v02, const Vector& n1, const Vector& n2) const
    {
        norm = v02 * n1 + v01 * n2;
    }
    inline void normInterp2(Vector& norm, const Vector& n1, const Vector& n2) const
    {
        norm = n1 + n2;
    }

    void buildMesh()
    {
        scalar.     clear();
        vert.       clear();
        vertNorm.   clear();

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
                    vertNorm.   push_back( Vector(0.f, 0.f, 0.f) );
                    scalar.     push_back( 0.f );
                }
            }
        }

        float _tmp = 0.f;
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
            if(scalar[i] > _tmp) _tmp = scalar[i];
        }
        isoValue = beta * _tmp;

        for(unsigned i=0; i<vertNorm.size(); i++)
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
                vertNorm[i].x = _n[0] - _n[1];
                vertNorm[i].y = _n[2] - _n[3];
                vertNorm[i].z = _n[4] - _n[5];
            }
        }
    }

    void buildTri()
    {
        tri.        clear();
        triNorm.    clear();

        for(unsigned i=0; i<numCube; i++)
        {
            unsigned _id[8];
            _id[0] = offset_vert(i);
            _id[1] = _id[0] + 1;
            _id[2] = _id[1] + i_vx + 1;
            _id[3] = _id[2] - 1;
            _id[4] = _id[0] + i_vx * i_vy;
            _id[5] = _id[4] + 1;
            _id[6] = _id[5] + i_vx + 1;
            _id[7] = _id[6] - 1;

            unsigned _index;
            _index  = unsigned(scalar[_id[0]] > isoValue);
            _index += unsigned(scalar[_id[1]] > isoValue) << 1;
            _index += unsigned(scalar[_id[2]] > isoValue) << 2;
            _index += unsigned(scalar[_id[3]] > isoValue) << 3;
            _index += unsigned(scalar[_id[4]] > isoValue) << 4;
            _index += unsigned(scalar[_id[5]] > isoValue) << 5;
            _index += unsigned(scalar[_id[6]] > isoValue) << 6;
            _index += unsigned(scalar[_id[7]] > isoValue) << 7;

            if(_index == 0x00 || _index == 0xff) continue;

            Point _edge[12];

            vertInterp(_edge[0], vert[_id[0]], vert[_id[1]], scalar[_id[0]], scalar[_id[1]], isoValue);
            vertInterp(_edge[1], vert[_id[1]], vert[_id[2]], scalar[_id[1]], scalar[_id[2]], isoValue);
            vertInterp(_edge[2], vert[_id[2]], vert[_id[3]], scalar[_id[2]], scalar[_id[3]], isoValue);
            vertInterp(_edge[3], vert[_id[3]], vert[_id[0]], scalar[_id[3]], scalar[_id[0]], isoValue);

            vertInterp(_edge[4], vert[_id[4]], vert[_id[5]], scalar[_id[4]], scalar[_id[5]], isoValue);
            vertInterp(_edge[5], vert[_id[5]], vert[_id[6]], scalar[_id[5]], scalar[_id[6]], isoValue);
            vertInterp(_edge[6], vert[_id[6]], vert[_id[7]], scalar[_id[6]], scalar[_id[7]], isoValue);
            vertInterp(_edge[7], vert[_id[7]], vert[_id[4]], scalar[_id[7]], scalar[_id[4]], isoValue);

            vertInterp(_edge[8], vert[_id[0]], vert[_id[4]], scalar[_id[0]], scalar[_id[4]], isoValue);
            vertInterp(_edge[9], vert[_id[1]], vert[_id[5]], scalar[_id[1]], scalar[_id[5]], isoValue);
            vertInterp(_edge[10], vert[_id[2]], vert[_id[6]], scalar[_id[2]], scalar[_id[6]], isoValue);
            vertInterp(_edge[11], vert[_id[3]], vert[_id[7]], scalar[_id[3]], scalar[_id[7]], isoValue);

            Vector _norm[12];

            normInterp(_norm[0], (_edge[0]-vert[_id[0]]).mag(), (vert[_id[1]]-_edge[0]).mag(), vertNorm[_id[0]], vertNorm[_id[1]]);
            normInterp(_norm[1], (_edge[1]-vert[_id[1]]).mag(), (vert[_id[2]]-_edge[1]).mag(), vertNorm[_id[1]], vertNorm[_id[2]]);
            normInterp(_norm[2], (_edge[2]-vert[_id[2]]).mag(), (vert[_id[3]]-_edge[2]).mag(), vertNorm[_id[2]], vertNorm[_id[3]]);
            normInterp(_norm[3], (_edge[3]-vert[_id[3]]).mag(), (vert[_id[0]]-_edge[3]).mag(), vertNorm[_id[3]], vertNorm[_id[0]]);

            normInterp(_norm[4], (_edge[4]-vert[_id[4]]).mag(), (vert[_id[5]]-_edge[4]).mag(), vertNorm[_id[4]], vertNorm[_id[5]]);
            normInterp(_norm[5], (_edge[5]-vert[_id[5]]).mag(), (vert[_id[6]]-_edge[5]).mag(), vertNorm[_id[5]], vertNorm[_id[6]]);
            normInterp(_norm[6], (_edge[6]-vert[_id[6]]).mag(), (vert[_id[7]]-_edge[6]).mag(), vertNorm[_id[6]], vertNorm[_id[7]]);
            normInterp(_norm[7], (_edge[7]-vert[_id[7]]).mag(), (vert[_id[4]]-_edge[7]).mag(), vertNorm[_id[7]], vertNorm[_id[4]]);

            normInterp(_norm[8], (_edge[8]-vert[_id[0]]).mag(), (vert[_id[4]]-_edge[8]).mag(), vertNorm[_id[0]], vertNorm[_id[4]]);
            normInterp(_norm[9], (_edge[9]-vert[_id[1]]).mag(), (vert[_id[5]]-_edge[9]).mag(), vertNorm[_id[1]], vertNorm[_id[5]]);
            normInterp(_norm[10], (_edge[10]-vert[_id[2]]).mag(), (vert[_id[6]]-_edge[10]).mag(), vertNorm[_id[2]], vertNorm[_id[6]]);
            normInterp(_norm[11], (_edge[11]-vert[_id[3]]).mag(), (vert[_id[7]]-_edge[11]).mag(), vertNorm[_id[3]], vertNorm[_id[7]]);

            unsigned _numVer = MC::numVertsTable[_index];

            for(unsigned j=0; j<_numVer; j++)
            {
                tri.        push_back( _edge[MC::triTable[_index][j]] );
                triNorm.    push_back( _norm[MC::triTable[_index][j]] );
            }
        }
    }

private:
    unsigned    numCube;
    unsigned    numVert;
    unsigned    numTri;
    unsigned    i_cx, i_cy, i_cz;
    unsigned    i_vx, i_vy, i_vz;

};

#endif // PARTTOTRI_H
