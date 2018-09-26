/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Released under CC BY-NC
*/
#version 330 core

layout(location = 0) in vec3 modelVertexPos;
layout(location = 1) in vec3 modelVertexNorm;
layout(location = 2) in float modelVertexColor;

out vec3 viewVertexNorm;
out float vertColor;

uniform mat4 projectionMat;
uniform mat4 viewModelMat;
uniform float pointRadius;
uniform float pointScale;

void main()
{
    vec3 posView = vec3(viewModelMat * vec4(modelVertexPos, 1.0f));
    viewVertexNorm = vec3( viewModelMat * vec4(modelVertexNorm, 0.0f) );

    float dist = length(posView);

    gl_Position = projectionMat * vec4(posView, 1.f);

    gl_PointSize = pointRadius * pointScale / dist;

    vertColor = modelVertexColor;
}
