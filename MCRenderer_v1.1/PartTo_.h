/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Published under CC BY-NC
*/
#ifndef PARTTO__H
#define PARTTO__H

#include <cstdio>
#include <vector>
#include "Point.h"
#include "BBox.h"
#include "Voxel.h"

class PartTo_
{
public:
    PartTo_(const int& _type = 0, const float& _voxWidth = 0.f) : type(_type), voxWidth(_voxWidth)
    {
        voxWidthInv = 1.f / _voxWidth;
    }
    ~PartTo_() {}

    inline unsigned offset_voxel(const Point& p) const
    {
        unsigned _x = (p.x - box.pMin.x) * voxWidthInv;
        unsigned _y = (p.y - box.pMin.y) * voxWidthInv;
        unsigned _z = (p.z - box.pMin.z) * voxWidthInv;

        return (_z * voxObj->i_cx * voxObj->i_cy) + (_y * voxObj->i_cx) + _x;
    }

    void buildVoxel()
    {
        ///init voxel
        if(voxObj != NULL) voxObj   ->  ~Voxel();

        unsigned _lenth  = ceil( (box.pMax.x - box.pMin.x) * voxWidthInv );
        unsigned _width  = ceil( (box.pMax.y - box.pMin.y) * voxWidthInv );
        unsigned _height = ceil( (box.pMax.z - box.pMin.z) * voxWidthInv );

        _lenth  = ( (_lenth == 0)   ? 1 : _lenth );
        _width  = ( (_width == 0)   ? 1 : _width );
        _height = ( (_height == 0)  ? 1 : _height );

        voxObj = new Voxel(_lenth, _width, _height);

        for(unsigned i=0; i<part.size(); i++)
        {
            unsigned _voxId = offset_voxel(part[i]);

            voxObj ->  eleList[_voxId]. push_back(i);
        }
    }

    bool operator << (char file_name[])
    {
        part.clear();

        unsigned    _num  = 0;
        FILE*       _fid   = fopen(file_name, "r");
        if(_fid == NULL) return 0;

        fscanf(_fid, "%f", &time);
        fscanf(_fid, "%u", &_num);
        if(_num > part.max_size()) return 0;

        for(unsigned i=0; i<_num; i++)
        {
            int __type;
            Point __pt;

            fscanf(_fid, "%d %f %f %f %*f %*f %*f %*f %*f", &__type, &__pt.x, &__pt.y, &__pt.z);
            if(__type != type) continue;
            part.   push_back( __pt );
        }

        fclose(_fid);

        buildBBox();

        Vector _center = box.pCenter() - Point(0.f, 0.f, 0.f);

        for(unsigned i=0; i<part.size(); i++)
        {
            part[i] -= _center;
        }
        box.pMin -= _center;
        box.pMax -= _center;

        return 1;
    }

public:
    int     type;
    float   time;

    std::vector<Point>  part;

    float   voxWidth, voxWidthInv;
    Voxel*  voxObj;

    BBox box;

private:
    void buildBBox()
    {
        box = BBox();
        for(unsigned i=0; i<part.size(); i++)
        {
            box += part[i];
        }
        box.Expand(0.05f);
    }
};

#endif // PARTTO__H
