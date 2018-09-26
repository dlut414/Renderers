/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Published under CC BY-NC
*/
#version 330 core

layout(location = 0) in vec3 modelVertexPos;
layout(location = 1) in vec3 modelVertexNorm;

out vec3 viewVertexNorm;

uniform mat4 projectionMat;
uniform mat4 viewModelMat;

void main()
{
    viewVertexNorm = vec3( viewModelMat * vec4(modelVertexNorm, 0.0f) );

    gl_Position = projectionMat * viewModelMat * vec4(modelVertexPos, 1.0f);

}

