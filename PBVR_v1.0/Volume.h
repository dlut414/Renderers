/*LICENSE*/
#ifndef VOLUME_H
#define VOLUME_H

#include "BBox.h"

class Volume
{
public:
    Volume(const BBox& _box) : box(_box)
    {}
    ~Volume() {};

    virtual void allocVert() = 0;
    virtual void freeVert() = 0;
    virtual float capacity() const = 0;

public:
    BBox box;

};

#endif // VOLUME_H
