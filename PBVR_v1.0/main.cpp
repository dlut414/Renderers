/*LICENCE*/
#include <iostream>

#include "typedef.h"
#include "Scene.h"
#include "Cube.h"
#include "PartGen.h"
#include "Renderer.h"
#include "include/common.h"

#define WIDTH 0.01f
#define PART_DIS 0.2f
#define DELTA_T 0.0001f

using namespace std;
using namespace Renderer;

int main(int argc, char** argv)
{
    //Scene<mytype::Integer, mytype::Int3, mytype::Real, mytype::Real3> scene_test;
    Cube*       cube_test = new Cube    ( BBox(Point(0.f, 0.f, 0.f), Point(0.2f, 0.2f, 0.2f)), WIDTH );

    for(unsigned i=0; i<(cube_test->numVert); i++)
    {
        cube_test->alpha[i] = 1.f;
    }

    unsigned height = cube_test->i_z;
    unsigned width  = cube_test->i_y;
    unsigned lenth  = cube_test->i_x;

    for(unsigned k=0; k<=height; k++)
    {
        for(unsigned j=0; j<=width; j++)
        {
            for(unsigned i=0; i<=lenth; i++)
            {
                unsigned __id = k * (width+1) * (lenth+1) + j * (lenth+1) + i;
                if(i<2) cube_test->normal[__id] = Vector(-1.f, 0.f, 0.f);
                else if(i>lenth-2) cube_test->normal[__id] = Vector(1.f, 0.f, 0.f);
                else if(j<2) cube_test->normal[__id] = Vector(0.f, -1.f, 0.f);
                else if(j>width-2) cube_test->normal[__id] = Vector(0.f, 1.f, 0.f);
                else if(k<2) cube_test->normal[__id] = Vector(0.f, 0.f, -1.f);
                else if(k>height-2) cube_test->normal[__id] = Vector(0.f, 0.f, 1.f);
                else
                {
                    cube_test->normal[__id] = Vector(0.f, 0.f, 0.f);
                    //cube_test->alpha[__id]  = 0.f;
                }
            }
        }
    }

    PartGen*    part_test = new PartGen (PART_DIS * WIDTH, DELTA_T, 0);
    part_test->genPart_MP(cube_test);

    cout << part_test->numPart << endl;

    numParticle     = part_test->numPart / REPEAT_LEVEL;
    dataSize        = numParticle * sizeof(Point);

    r3_vertex_pos_data  = part_test->part.  data();
    r3_vertex_norm_data = part_test->normal.data();

    Init(argc, argv);
    MainLoop();
    Final();

    return 0;
}
