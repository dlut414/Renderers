/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Published under CC BY-NC
*/
#ifndef VOXEL_H
#define VOXEL_H

#include <vector>

class Voxel
{
public:
    Voxel(const unsigned& _cx = 1, const unsigned& _cy = 1, const unsigned& _cz = 1)
    : i_cx(_cx), i_cy(_cy), i_cz(_cz)
    {
        numVox = i_cx * i_cy * i_cz;
        allocList();
    }
    ~Voxel()
    {
        deleteList();
    }

public:
    unsigned                 numVox;
    unsigned                 i_cx, i_cy, i_cz;
    std::vector<unsigned>*   eleList;
    std::vector<unsigned>*   neiList;

private:
    void allocList()
    {
        eleList = new std::vector<unsigned>[numVox];
        neiList = new std::vector<unsigned>[numVox];

        for(unsigned k=0; k<i_cz; k++)
        {
            for(unsigned j=0; j<i_cy; j++)
            {
                for(unsigned i=0; i<i_cx; i++)
                {
                    unsigned __id = k * i_cx * i_cy + j * i_cx + i;

                    neiList[__id].  push_back(__id);

                    if(i != 0) neiList[__id].   push_back(__id - 1);
                    if(j != 0) neiList[__id].   push_back(__id - i_cx);
                    if(k != 0) neiList[__id].   push_back(__id - i_cx * i_cy);
                    if(i != i_cx-1) neiList[__id].   push_back(__id + 1);
                    if(j != i_cy-1) neiList[__id].   push_back(__id + i_cx);
                    if(k != i_cz-1) neiList[__id].   push_back(__id + i_cx * i_cy);

                    if(i != 0      && j != 0      && k != 0) neiList[__id].   push_back(__id - i_cx * i_cy - i_cx - 1);
                    if(i != i_cx-1 && j != 0      && k != 0) neiList[__id].   push_back(__id - i_cx * i_cy - i_cx + 1);
                    if(i != i_cx-1 && j != i_cy-1 && k != 0) neiList[__id].   push_back(__id - i_cx * i_cy + i_cx + 1);
                    if(i != 0      && j != i_cy-1 && k != 0) neiList[__id].   push_back(__id - i_cx * i_cy + i_cx - 1);

                    if(i != 0      && j != 0      && k != i_cz-1) neiList[__id].   push_back(__id + i_cx * i_cy - i_cx - 1);
                    if(i != i_cx-1 && j != 0      && k != i_cz-1) neiList[__id].   push_back(__id + i_cx * i_cy - i_cx + 1);
                    if(i != i_cx-1 && j != i_cy-1 && k != i_cz-1) neiList[__id].   push_back(__id + i_cx * i_cy + i_cx + 1);
                    if(i != 0      && j != i_cy-1 && k != i_cz-1) neiList[__id].   push_back(__id + i_cx * i_cy + i_cx - 1);

                    if(i != 0      && j != 0     ) neiList[__id].   push_back(__id - i_cx - 1);
                    if(i != i_cx-1 && j != 0     ) neiList[__id].   push_back(__id - i_cx + 1);
                    if(i != i_cx-1 && j != i_cy-1) neiList[__id].   push_back(__id + i_cx + 1);
                    if(i != 0      && j != i_cy-1) neiList[__id].   push_back(__id + i_cx - 1);

                    if(i != 0      && k != 0     ) neiList[__id].   push_back(__id - i_cx * i_cy - 1);
                    if(i != i_cx-1 && k != 0     ) neiList[__id].   push_back(__id - i_cx * i_cy + 1);
                    if(i != i_cx-1 && k != i_cz-1) neiList[__id].   push_back(__id + i_cx * i_cy + 1);
                    if(i != 0      && k != i_cz-1) neiList[__id].   push_back(__id + i_cx * i_cy - 1);

                    if(j != 0      && k != 0     ) neiList[__id].   push_back(__id - i_cx * i_cy - i_cx);
                    if(j != i_cy-1 && k != 0     ) neiList[__id].   push_back(__id - i_cx * i_cy + i_cx);
                    if(j != i_cy-1 && k != i_cz-1) neiList[__id].   push_back(__id + i_cx * i_cy + i_cx);
                    if(j != 0      && k != i_cz-1) neiList[__id].   push_back(__id + i_cx * i_cy - i_cx);
                }
            }
        }
    }
    void deleteList()
    {
        delete[] eleList;
        delete[] neiList;
    }
};

#endif // VOXEL_H
