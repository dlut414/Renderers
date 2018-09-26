/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Released under CC BY-NC
*/
#ifndef DRAWPIXEL_H
#define DRAWPIXEL_H

#include "header.h"
#include "Draw_.h"
#include "Shader.h"

class DrawWater : public Draw_
{
public:
    DrawWater(Controller* state) : Draw_(state)
    {}
    ~DrawWater() {}

    void init(unsigned _numBBVert, Point* _BBVertPos, unsigned _numVolVert, Point* _volPos, Vector* _volNorm, float* _volData)
    {
        numBBVert = _numBBVert;
        BBVertPos = _BBVertPos;
        numVolVert = _numVolVert;
        volPos      = _volPos;
        volNorm     = _volNorm;
        volData     = _volData;

        Draw_::initFramebuffer(Draw_::fbo[0]);
        Draw_::initTexture_2D(frontFaceTex, Draw_::stateObj->u_width, Draw_::stateObj->u_height);
        Draw_::attachTex_2D_color0(frontFaceTex);
        //Draw_::initRenderbuffer(dbo[0], Draw_::stateObj->u_width, Draw_::stateObj->u_height);
        //Draw_::attachRenderbuffer_depth(dbo[0]);

        Draw_::initFramebuffer(Draw_::fbo[1]);
        Draw_::initTexture_2D(dirTex, Draw_::stateObj->u_width, Draw_::stateObj->u_height);
        Draw_::attachTex_2D_color0(dirTex);
        //Draw_::initRenderbuffer(dbo[1], Draw_::stateObj->u_width, Draw_::stateObj->u_height);
        //Draw_::attachRenderbuffer_depth(dbo[1]);

        Draw_::initVAO(Draw_::vao);
        Draw_::initVBO(Draw_::vbo[0]);
        Draw_::initVBO(Draw_::vbo[1]);
        Draw_::initVBO(Draw_::vbo[2]);
        Draw_::initVBO(Draw_::vbo[3]);
        Draw_::initVBO(Draw_::vbo[4]);
        Draw_::initVBO(Draw_::vbo[5]);

        int max_size = 0;
        glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &max_size);
        printf("max texture dimention: %d\n", max_size);
        printf("vx: %d, vy: %d, vz: %d\n", i_vx, i_vy, i_vz);

        Draw_::initTexture_3D_3f(volPosTex, i_vx, i_vy, i_vz, (void*)volPos);
        Draw_::initTexture_3D_3f(volNormTex, i_vx, i_vy, i_vz, (void*)volNorm);
        Draw_::initTexture_3D_1f(volDataTex, i_vx, i_vy, i_vz, (void*)volData);

        initShader();
    }

    void draw(GLuint& bgTex)
    {
        ///use program0
        glBindFramebuffer(GL_FRAMEBUFFER, Draw_::fbo[0]);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("fbo[0] not ready\n");
        glViewport(0, 0, Draw_::stateObj->u_width, Draw_::stateObj->u_height);
        glFrontFace(GL_CCW);

        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderObj.programID[0]);
        glDepthFunc(GL_LESS);

        glUniformMatrix4fv( shaderObj.matrixID[0], 1, GL_FALSE, &(Draw_::stateObj->m_mvp[0][0]) );
        glUniformMatrix4fv( shaderObj.matrixID[1], 1, GL_FALSE, &(Draw_::stateObj->m_mvpInv[0][0]) );

        glBindBuffer(GL_ARRAY_BUFFER, Draw_::vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, numBBVert*sizeof(Point), BBVertPos, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, numBBVert);

        glDisableVertexAttribArray(0);

        ///use program1
        glBindFramebuffer(GL_FRAMEBUFFER, Draw_::fbo[1]);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("fbo[1] not ready\n");
        glViewport(0, 0, Draw_::stateObj->u_width, Draw_::stateObj->u_height);
        glFrontFace(GL_CW);

        glClearDepth(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderObj.programID[1]);
        glDepthFunc(GL_GREATER);

        glUniformMatrix4fv( shaderObj.matrixID[2], 1, GL_FALSE, &(Draw_::stateObj->m_mvp[0][0]) );
        glUniformMatrix4fv( shaderObj.matrixID[3], 1, GL_FALSE, &(Draw_::stateObj->m_mvpInv[0][0]) );

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, frontFaceTex);
        glUniform1i(shaderObj.textureID[0], 0);

        glBindBuffer(GL_ARRAY_BUFFER, Draw_::vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, numBBVert*sizeof(Point), BBVertPos, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, numBBVert);

        glDisableVertexAttribArray(0);

        ///use program2
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("fbo_def not ready\n");
        glViewport(0, 0, Draw_::stateObj->u_width, Draw_::stateObj->u_height);
        glFrontFace(GL_CCW);

        //glClearDepth(1);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderObj.programID[2]);
        glDepthFunc(GL_LESS);

        glUniformMatrix4fv( shaderObj.matrixID[4], 1, GL_FALSE, &(Draw_::stateObj->m_mvp[0][0]) );
        glUniformMatrix4fv( shaderObj.matrixID[5], 1, GL_FALSE, &(Draw_::stateObj->m_viewModelMat[0][0]) );
        glUniform3f(glGetUniformLocation(shaderObj.programID[2], "pMin"), box.pMin.x, box.pMin.y, box.pMin.z);
        glUniform3f(glGetUniformLocation(shaderObj.programID[2], "pMax"), box.pMax.x, box.pMax.y, box.pMax.z);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, frontFaceTex);
        glUniform1i(shaderObj.textureID[1], 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, dirTex);
        glUniform1i(shaderObj.textureID[2], 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_3D, volPosTex);
        glUniform1i(shaderObj.textureID[3], 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_3D, volNormTex);
        glUniform1i(shaderObj.textureID[4], 3);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_3D, volDataTex);
        glUniform1i(shaderObj.textureID[5], 4);

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, bgTex);
        glUniform1i(shaderObj.textureID[6], 5);

        glBindBuffer(GL_ARRAY_BUFFER, Draw_::vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, numBBVert*sizeof(Point), BBVertPos, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, numBBVert);

        glDisableVertexAttribArray(0);
    }

public:
    GLuint frontFaceTex;
    GLuint dirTex;
    GLuint dbo[2];

    GLuint volPosTex;
    GLuint volNormTex;
    GLuint volDataTex;

    Shader shaderObj;

    BBox        box;
    unsigned    i_vx;
    unsigned    i_vy;
    unsigned    i_vz;

    unsigned numBBVert;
    Point* BBVertPos;
    unsigned numVolVert;
    Point* volPos;
    Vector* volNorm;
    float* volData;

private:
    void initShader()
    {
        shaderObj.programID[0] = shaderObj.LoadShader("./shader0/vertex.glsl", "./shader0/fragment.glsl");
        shaderObj.matrixID[0] = glGetUniformLocation( shaderObj.programID[0], "vMvp" );
        shaderObj.matrixID[1] = glGetUniformLocation( shaderObj.programID[0], "fMvpInv" );

        shaderObj.programID[1] = shaderObj.LoadShader("./shader1/vertex.glsl", "./shader1/fragment.glsl");
        shaderObj.matrixID[2] = glGetUniformLocation( shaderObj.programID[1], "vMvp" );
        shaderObj.matrixID[3] = glGetUniformLocation( shaderObj.programID[1], "fMvpInv" );
        shaderObj.textureID[0] = glGetUniformLocation( shaderObj.programID[1], "frontFaceTex" );

        shaderObj.programID[2] = shaderObj.LoadShader("./shader2/vertex.glsl", "./shader2/fragment.glsl");
        shaderObj.matrixID[4] = glGetUniformLocation( shaderObj.programID[2], "vMvp" );
        shaderObj.matrixID[5] = glGetUniformLocation( shaderObj.programID[2], "fMv" );
        shaderObj.textureID[1] = glGetUniformLocation( shaderObj.programID[2], "frontFaceTex" );
        shaderObj.textureID[2] = glGetUniformLocation( shaderObj.programID[2], "dirTex" );
        shaderObj.textureID[3] = glGetUniformLocation( shaderObj.programID[2], "volPosTex" );
        shaderObj.textureID[4] = glGetUniformLocation( shaderObj.programID[2], "volNormTex" );
        shaderObj.textureID[5] = glGetUniformLocation( shaderObj.programID[2], "volDataTex" );
        shaderObj.textureID[6] = glGetUniformLocation( shaderObj.programID[2], "bgTex" );
    }
};

#endif // DRAWPIXEL_H
