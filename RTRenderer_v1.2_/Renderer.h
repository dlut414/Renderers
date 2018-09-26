/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Released under CC BY-NC
*/
#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include <cstdio>
#include "header.h"
#include "Camera.h"
#include "common.h"
#include "../common/typedef/Point.h"
#include "PartToCube.h"
#include "Controller.h"
#include "DrawWater.h"
#include "DrawTriangle.h"

namespace Renderer
{

std::vector<Point> BBVertPos;
std::vector<Point> tri;

Controller*     stateObj;
PartToCube*     sceneObj;
DrawWater*      renderObj1;
DrawTriangle*   renderObj2;

void Init(int argc, char** argv);
void MainLoop();
void Final();

void InitOBJ();

void Fps();
void myMouse(int, int, int, int);
void myMotion(int, int);
void myMouseWheel(int, int, int, int);
void myReshape(int, int);
void myKeyboard(unsigned char, int, int);
void myDisplay();


void InitOBJ()
{
    char str[256];

    ///init sceneObj
    sprintf(str, "../input/%04d.out", stateObj->i_file);
    std::cout << (*sceneObj << str) << std::endl;

    sceneObj->build();

    std::cout << "# of vertex: " << sceneObj->vert.size() << std::endl;
    std::cout << "max isoValue: " << sceneObj->iso << std::endl;
    std::cout << "boundary1: " << sceneObj->box.pMin.x << " " << sceneObj->box.pMin.y << " " << sceneObj->box.pMin.z << " " << std::endl;
    std::cout << "boundary2: " << sceneObj->box.pMax.x << " " << sceneObj->box.pMax.y << " " << sceneObj->box.pMax.z << " " << std::endl;

    BBVertPos.clear();

    ///init renderObj1
    renderObj1->box  = sceneObj->box;
    renderObj1->i_vx = sceneObj->i_vx;
    renderObj1->i_vy = sceneObj->i_vy;
    renderObj1->i_vz = sceneObj->i_vz;

    renderObj1->init(float(sceneObj->voxWidth),
                     unsigned(sceneObj->part.size()),
                    (Point*)(sceneObj->part.data()),
                    unsigned(sceneObj->vert.size()),
                    (Point*)(sceneObj->vert.data()),
                    (Vector*)(sceneObj->norm.data()),
                    (float*)(sceneObj->scalar.data()));

    ///init renderObj2
    std::vector<int> vertId;
    std::vector<Point> vert;
    vertId.clear();
    vert.clear();
    tri.clear();

    int nTri, nP;
    FILE* fid = fopen("../geometry/hexahedron.geo", "r");
    fscanf(fid, "%*s");
    fscanf(fid, "%*s %*s");
    fscanf(fid, "%d", &nTri);
    int tmpId[3];
    for(int i=0; i<nTri; i++)
    {
        fscanf(fid, "%d %d %d", &tmpId[0], &tmpId[1], &tmpId[2]);
        vertId.push_back(tmpId[0]); vertId.push_back(tmpId[1]); vertId.push_back(tmpId[2]);
    }
    fscanf(fid, "%d", &nP);
    Point tmpP;
    for(int i=0; i<nP; i++)
    {
        fscanf(fid, "%f %f %f", &tmpP.x, &tmpP.y, &tmpP.z);
        vert.push_back(tmpP);
    }
    for(int i=0; i<nTri*3; i++)
    {
        tri.push_back(vert[vertId[i]]);
    }
    fclose(fid);
    renderObj2->init(tri.size(), (Point*)tri.data());
}

void InitGL(int argc, char** argv)
{
    glutInit                (&argc, argv);
    glutInitDisplayMode     (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition  (0, 0);
    glutInitWindowSize      (stateObj->u_width, stateObj->u_height);
    glutCreateWindow        ("RTRenderer");
    //glLightModeli           (GL_LIGHT_MODEL_TWO_SIDE, 1);
    glutMouseFunc           (myMouse);
    glutMotionFunc          (myMotion);
    //glutMouseWheelFunc      (myMouseWheel);
    glutReshapeFunc         (myReshape);
    glutKeyboardFunc        (myKeyboard);
    glutDisplayFunc         (myDisplay);

    glEnable                (GL_TEXTURE_1D);
    glEnable                (GL_TEXTURE_2D);
    glEnable                (GL_TEXTURE_3D);
    glEnable                (GL_CULL_FACE);
    //glDisable               (GL_CULL_FACE);
    glFrontFace             (GL_CCW);
    glEnable                (GL_POINT_SPRITE_ARB);
    glEnable                (GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable                (GL_DEPTH_TEST);
    glDepthFunc             (GL_LESS);
    glEnable                (GL_ALPHA_TEST);
    glAlphaFunc             (GL_GREATER, 0.f);
    //glEnable                (GL_BLEND);
    //glBlendFunc             (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor            (1.f, 1.f, 1.f, 0.f);

    glewInit              ();
}

void Fps() {}

void MainLoop() { glutMainLoop(); }

void Final() {}

void myMouse(int button, int s, int x, int y)
{
    stateObj->clickMouse(button, s, x, y);
}

void myMotion(int x, int y)
{
    stateObj->moveMouse(x, y);

    glutPostRedisplay();
}

void myMouseWheel(int button, int dir, int x, int y)
{
    stateObj->rollMouse(button, dir, x, y);
}

void myReshape(int width, int height)
{
    glViewport      (0, 0, width, width);
    glMatrixMode    (GL_MODELVIEW);
    glLoadIdentity  ();
    gluPerspective  (-90.0f, float(stateObj->u_width)/float(stateObj->u_height), 1.0f, 100.0f);

    stateObj->reshapeWindow();
}

void myKeyboard(unsigned char key, int a, int b)
{
    glutPostRedisplay();

    stateObj->pressKey(key, a, b);

    if(stateObj->b_init) InitOBJ();

    stateObj->b_init = false;
}

void myDisplay()
{
    /*
    if(!b_stop)
    {
        b_dirty = true;
    }
    */
    glm::mat4 modelMatrix = glm::translate  ( glm::mat4(1.0f), stateObj->m_pan )
                          * glm::toMat4     ( stateObj->m_rotation )
                          * glm::scale      ( glm::mat4(1.0f), stateObj->m_scale );

    stateObj->m_viewModelMat      = stateObj->m_camera.GetViewMatrix() * modelMatrix;
    stateObj->m_projectionMat     = stateObj->m_camera.GetProjectionMatrix();
    stateObj->m_projectionMatInv  = glm::inverse( stateObj->m_projectionMat );
    stateObj->m_mvp               = stateObj->m_projectionMat * stateObj->m_viewModelMat;
    stateObj->m_mvpInv            = glm::inverse( stateObj->m_mvp );

    renderObj2->draw();
    renderObj1->draw(renderObj2->bgTex);

    glutSwapBuffers     ();
    glutReportErrors    ();

    Fps();

    if(stateObj->b_dirty)
    {
        glutPostRedisplay();
        stateObj->b_dirty = false;
    }

    if(stateObj->b_leave)
    {
        glutLeaveMainLoop();
    }
}


} //namespace Renderer

#endif // RENDERER_H_INCLUDED
