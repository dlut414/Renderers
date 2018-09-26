/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Released under CC BY-NC
*/
#include <iostream>
#include "Renderer.h"

#define TYPE        0
#define RZERO       0.015f
#define BETA        0.85f
#define MESHWIDTH   0.006f

#define START       0

using namespace std;
using namespace Renderer;

int main(int argc, char** argv)
{
    sceneObj   = new PartToCube(TYPE, RZERO, MESHWIDTH);
    stateObj   = new Controller(START);
    renderObj1 = new DrawWater(stateObj);
    renderObj2 = new DrawTriangle(stateObj);

    InitGL(argc, argv);
    InitOBJ();
    MainLoop();
    Final();
}
