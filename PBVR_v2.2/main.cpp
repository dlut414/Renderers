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
#include "Renderer.h"

#define PART_DIS    0.25f
#define DELTA_T     4.f

#define TYPE        0
#define RZERO       0.010f
#define BETA        0.85f
#define MESHWIDTH   0.006f

#define START       5

using namespace std;

int main(int argc, char** argv)
{
    Renderer::i_file = START;
    Renderer::f_pointRadius = PART_DIS * MESHWIDTH;

    Renderer::data_obj = new PartToPart(TYPE, RZERO, MESHWIDTH, BETA, 1.15f);
    Renderer::part_obj = new PartGen(PART_DIS * MESHWIDTH, DELTA_T, 0);

    Renderer::InitOBJ   ();
    Renderer::Init      (argc, argv);
    Renderer::MainLoop  ();
    Renderer::Final     ();

    return 0;
}

