/*LICENCE*/
#include <iostream>

#include "Cube.h"
#include "PartGen.h"
#include "PartToCube.h"
#include "Renderer.h"
#include "common.h"

#define PART_DIS    0.25f
#define DELTA_T     4.f

#define TYPE        0
#define RZERO       0.012f
#define BETA        0.7f
#define MESHWIDTH   0.006f

#define START       5

using namespace std;

int main(int argc, char** argv)
{
    Renderer::i_file = START;
    Renderer::f_pointRadius = PART_DIS * MESHWIDTH;

    Renderer::cube_obj = new PartToCube(TYPE, RZERO, MESHWIDTH);
    Renderer::part_obj = new PartGen(PART_DIS * MESHWIDTH, DELTA_T, BETA, 0);

    Renderer::InitOBJ   ();
    Renderer::Init      (argc, argv);
    Renderer::MainLoop  ();
    Renderer::Final     ();

    return 0;
}
