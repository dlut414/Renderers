/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Released under CC BY-NC
*/
#ifndef DRAWTRIANGLE_H
#define DRAWTRIANGLE_H

#include "header.h"
#include "Draw_.h"
#include "Shader.h"

class DrawTriangle : public Draw_
{
public:
    DrawTriangle(Controller* state) : Draw_(state)
    {}
    ~DrawTriangle() {}

    void init(unsigned _numVert, Point* _vert)
    {
        numVert = _numVert;
        vert    = _vert;

        Draw_::initFramebuffer(Draw_::fbo[0]);
        Draw_::initRenderbuffer(dbo, stateObj->u_width, stateObj->u_height);
        attachRenderbuffer_depth(dbo);
        Draw_::initTexture_2D(bgTex, stateObj->u_width, stateObj->u_height);
        attachTex_2D_color0(bgTex);

        Draw_::initVAO(Draw_::vao);
        Draw_::initVBO(Draw_::vbo[0]);

        initShader();
    }

    void draw()
    {
        ///use program3
        glBindFramebuffer(GL_FRAMEBUFFER, fbo[0]);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("fbo[0] not ready\n");
        glViewport(0, 0, Draw_::stateObj->u_width, Draw_::stateObj->u_height);
        glFrontFace(GL_CCW);

        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderObj.programID[0]);
        glDepthFunc(GL_LESS);

        glUniformMatrix4fv( shaderObj.matrixID[0], 1, GL_FALSE, &(Draw_::stateObj->m_mvp[0][0]) );

        glBindBuffer(GL_ARRAY_BUFFER, Draw_::vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, numVert*sizeof(Point), vert, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, numVert);

        glDisableVertexAttribArray(0);

        ///draw to framebuffer0
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("fbo_def not ready\n");
        glViewport(0, 0, Draw_::stateObj->u_width, Draw_::stateObj->u_height);
        glFrontFace(GL_CCW);

        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);

        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, numVert);

        glDisableVertexAttribArray(0);
    }

public:
    GLuint dbo;
    GLuint bgTex;

    Shader shaderObj;

    unsigned numVert;
    Point* vert;

private:
    void initShader()
    {
        shaderObj.programID[0] = shaderObj.LoadShader("./shader3/vertex.glsl", "./shader3/fragment.glsl");
        shaderObj.matrixID[0] = glGetUniformLocation( shaderObj.programID[0], "vMvp" );
    }
};

#endif // DRAWTRIANGLE_H
