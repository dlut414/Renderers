/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Released under CC BY-NC
*/
#include <iostream>

#include "Cube.h"
#include "PartGen.h"
#include "PartToCube.h"
#include "PartToPart.h"
#include "PartToTri.h"
#include "Renderer.h"

#define PART_DIS    0.5f
#define DELTA_T     2.f

#define TYPE1       1
#define TYPE2       2
#define RZERO       2.0*6.284500e-5
#define BETA        0.85f
#define MESHWIDTH   6.284500e-5

#define START       93

using namespace std;

int main(int argc, char** argv)
{
    Renderer::i_file = START;
    Renderer::f_pointRadius = PART_DIS * MESHWIDTH;

    Renderer::data_obj = new PartToPart(TYPE1, RZERO, MESHWIDTH, BETA, 1.15f);
    Renderer::part_obj = new PartGen(PART_DIS * MESHWIDTH, DELTA_T, 0);

    Renderer::tria_obj = new PartToTri(TYPE2, 1.1f*RZERO, BETA, MESHWIDTH);

    Renderer::InitOBJ   ();
    Renderer::Init      (argc, argv);
    Renderer::MainLoop  ();
    Renderer::Final     ();

    return 0;
}

