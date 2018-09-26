/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Released under CC BY-NC
*/
#version 330 core

layout(location = 0) in vec3 vPos;

out vec3 fPos;

void main()
{
    viewVertexNorm = vec3( viewModelMat * vec4(modelVertexNorm, 0.0f) );

    gl_Position = projectionMat * viewModelMat * vec4(modelVertexPos, 1.0f);

}

