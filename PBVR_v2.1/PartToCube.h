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
#include "Cube.h"
#include "PartTo_.h"

class PartToCube : public PartTo_
{
public:
    PartToCube(const int& _type = 0, const float& _voxWidth = 1.f, const float& _meshWidth = 1.f)
    : PartTo_(_type, _voxWidth)
    {
        obj = new Cube(PartTo_::box, _meshWidth);
    }
    ~PartToCube()
    {
        obj ->  ~Cube();
    }

    void build()
    {
        PartTo_::buildVoxel  ();
        buildMesh   ();
    }
    void update()
    {}

public:
    Cube*    obj;

private:
    inline float weight_cubic(const float& _r0, const float& _r)
    {
        float __q = _r / _r0;

        if(__q <= 0.5f) return 8.0f * Common::OVERPI * (1 - 6*__q*__q + 6*__q*__q*__q);
        else if(__q <= 1.0f) return 16.0f * Common::OVERPI * (1.0f-__q) * (1.0f-__q) * (1.0f-__q);
        else return 0.0f;
    }

    void buildMesh()
    {
        obj->box = PartTo_::box;

        obj->build();

        float _tmp = 0.f;
        for(unsigned i=0; i<(obj->numVert); i++)
        {
            Point       _verti  = obj->vert[i];
            unsigned    _vox    = offset_voxel(_verti);

            for(unsigned nb = 0; nb < PartTo_::voxObj->neiList[_vox].size(); nb++)
            {
                unsigned _nbid = PartTo_::voxObj ->  neiList[_vox][nb];

                for(unsigned j = 0; j < PartTo_::voxObj->eleList[_nbid].size(); j++)
                {
                    unsigned _partjId = PartTo_::voxObj  ->  eleList[_nbid][j];

                    Point __partj   =   PartTo_::part[_partjId];
                    Vector __velj   =   PartTo_::vel[_partjId];
                    float __dr      =   (__partj - _verti).mag();
                    obj->alpha[i]   +=  weight_cubic(PartTo_::voxWidth, __dr);
                    obj->vel[i]     +=  weight_cubic(PartTo_::voxWidth, __dr) * __velj;
                }
            }
            if(obj->alpha[i] > _tmp) _tmp = obj->alpha[i];
            /*
            for(unsigned j=0; j<part.size(); j++)
            {
                Point __partj   = part[j];
                float __rr      = (__partj - _verti).mag2();
                obj.alpha[i] += weight_cubic(voxWidth, sqrt(__rr));
            }
            */
        }
        _tmp = (_tmp == 0) ? 1.f : _tmp;
        for(unsigned i=0; i<(obj->numVert); i++) obj->alpha[i] /= _tmp;

        for(unsigned i=0; i<(obj->numVert); i++)
        {
            unsigned    _vx  = obj->i_vx;
            unsigned    _vy  = obj->i_vy;
            unsigned    _num = obj->numVert;

            if( i + _vx * _vy < _num && i >= _vx * _vy )
            {
                float _n[6];
                _n[0] = obj->alpha[i-1];
                _n[1] = obj->alpha[i+1];
                _n[2] = obj->alpha[i-_vx];
                _n[3] = obj->alpha[i+_vx];
                _n[4] = obj->alpha[i-_vx*_vy];
                _n[5] = obj->alpha[i+_vx*_vy];

                ///out pointing
                obj->norm[i].x = _n[0] - _n[1];
                obj->norm[i].y = _n[2] - _n[3];
                obj->norm[i].z = _n[4] - _n[5];
            }
        }
    }

};

#endif // PARTTOCUBE_H
