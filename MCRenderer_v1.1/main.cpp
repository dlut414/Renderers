/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Published under CC BY-NC
*/
#include <iostream>
#include "BBox.h"
#include "PartToTri.h"
#include "Renderer.h"

#define TYPE        1
#define RZERO       0.13125f
#define MESHWIDTH   0.0625f
#define BETA        0.5f

#define START       7

using namespace std;

int main(int argc, char** argv)
{
    Renderer::i_file = START;

    Renderer::obj = new PartToTri(TYPE, RZERO, BETA, MESHWIDTH);

    Renderer::InitOBJ();
    Renderer::Init(argc, argv);
    Renderer::MainLoop();
    Renderer::Final();
}
