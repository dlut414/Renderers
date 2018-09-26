/*LICENCE*/
#include <iostream>

#include "Cube.h"
#include "PartGen.h"
#include "PartToCube.h"
#include "Renderer.h"
#include "common.h"

#define PART_DIS    0.2f
#define DELTA_T     0.001f

#define TYPE        2
#define RZERO       0.0005f
#define BETA        0.85f
#define MESHWIDTH   0.0002f

#define START       31

using namespace std;

int main(int argc, char** argv)
{
    Renderer::i_file = START;

    Renderer::cube_obj = new PartToCube(TYPE, RZERO, MESHWIDTH);
    Renderer::part_obj = new PartGen(PART_DIS * MESHWIDTH, DELTA_T, 0);

    Renderer::InitOBJ();
    Renderer::Init(argc, argv);
    Renderer::MainLoop();
    Renderer::Final();

    return 0;
}
