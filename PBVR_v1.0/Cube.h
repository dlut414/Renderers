/*LICENSE*/
#ifndef CUBE_H
#define CUBE_H

#include <cmath>
#include <vector>
#include "Volume.h"
#include "Vector.h"

class Cube : public Volume
{
public:
    Cube(const BBox& _box = BBox(Point(0,0,0)), const float& _width = 1.f) : Volume(_box), width(_width)
    {
        widthInv = (_width == 0) ? 0.f : (1.f / _width);
        i_x = floor( (_box.pMax.x - _box.pMin.x) * widthInv );
        i_y = floor( (_box.pMax.y - _box.pMin.y) * widthInv );
        i_z = floor( (_box.pMax.z - _box.pMin.z) * widthInv );

        if(i_x == 0) i_x++;
        if(i_y == 0) i_y++;
        if(i_z == 0) i_z++;

        numCube = i_x * i_y * i_z;
        numVert = (i_x + 1) * (i_y + 1) * (i_z + 1);

        allocVert();
    }
    ~Cube()
    {
        freeVert();
    }

    float capacity() const
    {
        return width * width * width;
    }

    unsigned offset_cube(const Point& p) const
    {
        unsigned _x = (p.x - Volume::box.pMin.x) * widthInv;
        unsigned _y = (p.y - Volume::box.pMin.y) * widthInv;
        unsigned _z = (p.z - Volume::box.pMin.z) * widthInv;

        return (_z * i_x * i_y) + (_y * i_x) + _x;
    }

    unsigned offset_vert(const Point& p) const
    {
        unsigned _x = (p.x - Volume::box.pMin.x) * widthInv;
        unsigned _y = (p.y - Volume::box.pMin.y) * widthInv;
        unsigned _z = (p.z - Volume::box.pMin.z) * widthInv;

        return (_z * (i_x + 1) * (i_y + 1)) + (_y * (i_x + 1)) + _x;
    }

    BBox bound(const unsigned& id) const
    {
        unsigned _z = id / (i_x * i_y);
        unsigned _y = ( id - (_z * (i_x * i_y)) ) / i_x;
        unsigned _x = id - (_z * (i_x * i_y)) - _y * i_x;

        float _fx = Volume::box.pMin.x + _x * width;
        float _fy = Volume::box.pMin.y + _y * width;
        float _fz = Volume::box.pMin.z + _z * width;

        return BBox(Point(_fx, _fy, _fz), Point(_fx+width, _fy+width, _fz+width));
    }
    BBox bound(const unsigned& _idx, const unsigned& _idy, const unsigned& _idz) const
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
        _id[2] = _id[1] + i_x + 1;
        _id[3] = _id[2] - 1;
        _id[4] = _id[0] + (i_x + 1) * (i_y + 1);
        _id[5] = _id[4] + 1;
        _id[6] = _id[5] + i_x + 1;
        _id[7] = _id[6] - 1;

        Vector _v = Vector( (p - vertex[_id[0]]) * widthInv );

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
        _id[2] = _id[1] + i_x + 1;
        _id[3] = _id[2] - 1;
        _id[4] = _id[0] + (i_x + 1) * (i_y + 1);
        _id[5] = _id[4] + 1;
        _id[6] = _id[5] + i_x + 1;
        _id[7] = _id[6] - 1;

        Vector _v = Vector( (p - vertex[_id[0]]) * widthInv );

        _ret += 0.125f * (1 - _v.x) * (1 - _v.y) * (1 - _v.z) * normal[_id[0]];
        _ret += 0.125f * (1 + _v.x) * (1 - _v.y) * (1 - _v.z) * normal[_id[1]];
        _ret += 0.125f * (1 + _v.x) * (1 + _v.y) * (1 - _v.z) * normal[_id[2]];
        _ret += 0.125f * (1 - _v.x) * (1 + _v.y) * (1 - _v.z) * normal[_id[3]];
        _ret += 0.125f * (1 - _v.x) * (1 - _v.y) * (1 + _v.z) * normal[_id[4]];
        _ret += 0.125f * (1 + _v.x) * (1 - _v.y) * (1 + _v.z) * normal[_id[5]];
        _ret += 0.125f * (1 + _v.x) * (1 + _v.y) * (1 + _v.z) * normal[_id[6]];
        _ret += 0.125f * (1 - _v.x) * (1 + _v.y) * (1 + _v.z) * normal[_id[7]];

        return _ret;
    }

public:
    float width, widthInv;
    unsigned i_x, i_y, i_z;
    unsigned numCube;
    unsigned numVert;

/*
    Point*  vertex;
    float*  alpha;
    Vector* normal;
*/
    std::vector<Point>  vertex;
    std::vector<float>  alpha;
    std::vector<Vector> normal;

private:
    void allocVert()
    {
/*
        vertex = new Point [numVert];
        alpha  = new float [numVert];
        normal = new Vector[numVert];

        unsigned _tmp = 0;
*/
        for(unsigned k=0; k<i_z+1; k++)
        {
            for(unsigned j=0; j<i_y+1; j++)
            {
                for(unsigned i=0; i<i_x+1; i++)
                {
                    vertex.push_back( Point(Volume::box.pMin.x + i*width,
                                            Volume::box.pMin.y + j*width,
                                            Volume::box.pMin.z + k*width) );

                    normal.push_back( Vector(0.f, 0.f, 0.f) );

                    alpha. push_back( 0.f );
/*
                    vertex[_tmp]    = Point(Volume::box.pMin.x + i*width,
                                            Volume::box.pMin.y + j*width,
                                            Volume::box.pMin.z + k*width);
                    alpha[_tmp]     = 0.f;

                    normal[_tmp]    = Vector(0.f, 0.f, 0.f);

                    _tmp++;
*/
                }
            }
        }
    }
    void freeVert()
    {
        vertex.~vector();
        alpha.~vector();
        normal.~vector();
/*
        delete[] vertex;    vertex = NULL;
        delete[] alpha;     alpha  = NULL;
*/
    }

};

#endif // CUBE_H
