/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Released under CC BY-NC
*/
#ifndef PARTTOPART_H
#define PARTTOPART_H

#include <cmath>
#include <vector>
#include "common.h"
#include "PartTo_.h"

class PartToPart : public PartTo_
{
public:
    PartToPart(const int& _type = 0, const float& _voxWidth = 1.f, const float& _rZero = 1.f, const float& _aMin = 0.f, const float& _aMax = 1.f)
    : PartTo_(_type, _voxWidth), rZero(_rZero), aMin(_aMin), aMax(_aMax)
    {
        maxAlpha = 0.f;
        maxVel  = 0.f;
    }
    ~PartToPart()
    {}

    void build()
    {
        PartTo_::buildVoxel();
        buildPart();
    }
    void update() {}

    inline const float capacity() const
    {
        return (4.f / 3.f * Common::PI * rZero * rZero * rZero);
    }

    const float inpAlpha(const Point& pt) const
    {
        unsigned    _vox    = PartTo_::offset_voxel(pt);
        float       _alpha  = 0.f;

        for(unsigned nb = 0; nb < PartTo_::voxObj->neiList[_vox].size(); nb++)
        {
            unsigned _nbid = PartTo_::voxObj ->  neiList[_vox][nb];

            for(unsigned j = 0; j < PartTo_::voxObj->eleList[_nbid].size(); j++)
            {
                unsigned _partjId = PartTo_::voxObj  ->  eleList[_nbid][j];
                //if(_partjId == i) continue;

                Point __partj   =   PartTo_::part[_partjId];
                float __dr      =   (__partj - pt).mag();
                _alpha          +=  weight_cubic(PartTo_::voxWidth, __dr);
            }
        }

        return _alpha;
    }

    const Vector inpNormal(const Point& pt) const
    {
        unsigned    _vox    = PartTo_::offset_voxel(pt);
        Vector      _norm   = Vector(0.f, 0.f, 0.f);

        for(unsigned nb = 0; nb < PartTo_::voxObj->neiList[_vox].size(); nb++)
        {
            unsigned _nbid = PartTo_::voxObj ->  neiList[_vox][nb];

            for(unsigned j = 0; j < PartTo_::voxObj->eleList[_nbid].size(); j++)
            {
                unsigned _partjId = PartTo_::voxObj  ->  eleList[_nbid][j];
                //if(_partjId == i) continue;

                Point   __partj   = PartTo_::part[_partjId];
                Vector  __rij     = pt - __partj;
                float   __dr      = __rij.mag();

                _norm += alpha[_partjId] * weight_cubic_grad(PartTo_::voxWidth, __dr) / __dr * __rij;
            }
        }

        return _norm;
    }

    const float inpVel(const Point& pt) const
    {
        unsigned    _vox    = PartTo_::offset_voxel(pt);
        float       _vel    = 0.f;
        float       _weight = 0.f;

        for(unsigned nb = 0; nb < PartTo_::voxObj->neiList[_vox].size(); nb++)
        {
            unsigned _nbid = PartTo_::voxObj ->  neiList[_vox][nb];

            for(unsigned j = 0; j < PartTo_::voxObj->eleList[_nbid].size(); j++)
            {
                unsigned _partjId = PartTo_::voxObj  ->  eleList[_nbid][j];
                //if(_partjId == i) continue;

                Point __partj   =   PartTo_::part[_partjId];
                float __dr      =   (__partj - pt).mag();
                float __w       =   weight_cubic(PartTo_::voxWidth, __dr);
                _weight        +=   __w;
                _vel           +=   PartTo_::vel[_partjId].mag() * __w;
            }
        }
        _weight = (_weight == 0.f) ? 1.f : _weight;
        return _vel / _weight;
    }

public:
    float   rZero;
    float   aMin;
    float   aMax;
    float   maxAlpha;
    float   maxVel;
    std::vector<float>  alpha;
    std::vector<Vector> norm;

private:
    inline const float weight_cubic(const float& _r0, const float& _r) const
    {
        float __q = _r / _r0;

        if     (__q <= 0.5f) return  8.f * Common::OVERPI * (1.f - 6.f*__q*__q + 6.f*__q*__q*__q);
        else if(__q <= 1.0f) return 16.f * Common::OVERPI * (1.f-__q) * (1.f-__q) * (1.f-__q);
        else return 0.f;
    }
    inline const float weight_cubic_grad(const float& _r0, const float& _r) const
    {
        float __r0Inv = 1.f / _r0;
        float __q = _r * __r0Inv;

        if     (__q <= 0.5f) return  48.f * Common::OVERPI * __r0Inv * (-2.f*__q + 3.f*__q*__q);
        else if(__q <= 1.0f) return -48.f * Common::OVERPI * __r0Inv * (1.f-__q) * (1.f-__q);
        else return 0.f;
    }
    inline const float weight_mps(const float& _r0, const float& _r) const
    {
        if(_r >= _r0) return 0.f;
        else return (_r0 / _r - 1.f);
    }

    bool buildPart()
    {
        if( PartTo_::part.size() > alpha.max_size() || PartTo_::part.size() > norm.max_size() ) return 0;

        alpha.  clear();
        norm.   clear();

        maxAlpha = 0.f;
        for(unsigned i=0; i<(PartTo_::part.size()); i++)
        {
            Point       _parti  = PartTo_::part[i];
            unsigned    _vox    = PartTo_::offset_voxel(_parti);
            float       _alpha  = 0.f;

            for(unsigned nb = 0; nb < PartTo_::voxObj->neiList[_vox].size(); nb++)
            {
                unsigned _nbid = PartTo_::voxObj ->  neiList[_vox][nb];

                for(unsigned j = 0; j < PartTo_::voxObj->eleList[_nbid].size(); j++)
                {
                    unsigned _partjId = PartTo_::voxObj  ->  eleList[_nbid][j];
                    if(_partjId == i) continue;

                    Point __partj   =   PartTo_::part[_partjId];
                    float __dr      =   (__partj - _parti).mag();
                    _alpha          +=  weight_cubic(PartTo_::voxWidth, __dr);
                }
            }
            /*
            for(unsigned j=0; j<part.size(); j++)
            {
                Point __partj   = part[j];
                float __dr      = (__partj - _parti).mag();
                _alpha         += weight_cubic(PartTo_::voxWidth, __dr);
            }
            */
            alpha.push_back(_alpha);
            if(_alpha > maxAlpha) maxAlpha = _alpha;
        }

        maxAlpha = (maxAlpha == 0.f) ? 1.f : maxAlpha;
        //for(unsigned i=0; i<alpha.size(); i++) alpha[i] /= maxAlpha;

        for(unsigned i=0; i<(PartTo_::part.size()); i++)
        {
            Point       _parti  = PartTo_::part[i];
            unsigned    _vox    = PartTo_::offset_voxel(_parti);
            Vector      _norm   = Vector(0.f, 0.f, 0.f);

            for(unsigned nb = 0; nb < PartTo_::voxObj->neiList[_vox].size(); nb++)
            {
                unsigned _nbid = PartTo_::voxObj ->  neiList[_vox][nb];

                for(unsigned j = 0; j < PartTo_::voxObj->eleList[_nbid].size(); j++)
                {
                    unsigned _partjId = PartTo_::voxObj  ->  eleList[_nbid][j];
                    if(_partjId == i) continue;

                    Point   __partj   = PartTo_::part[_partjId];
                    Vector  __rij     = __partj - _parti;
                    float   __rr      = __rij.mag2();
                    float   __dr      = sqrt(__rr);

                    _norm += (weight_cubic(PartTo_::voxWidth, __dr) * (alpha[_partjId] - alpha[i]) / __rr) * __rij;
                }
            }
            /*
            for(unsigned j=0; j<part.size(); j++)
            {
                Point __partj   = part[j];
                float __dr      = (__partj - _parti).mag();
                _alpha         += weight_cubic(PartTo_::voxWidth, __dr);
            }
            */
            norm.push_back(_norm);
        }

        maxVel = 0.f;
        for(unsigned i=0; i<PartTo_::vel.size(); i++)
        {
            float _tmp = PartTo_::vel[i].mag();
            maxVel = (maxVel > _tmp) ? maxVel : _tmp;
        }
        maxVel = (maxVel == 0.f) ? 1.f : maxVel;
        //for(unsigned i=0; i<PartTo_::vel.size(); i++) PartTo_::vel[i] /= maxVel;

        return 1;
    }

};

#endif // PARTTOPART_H
