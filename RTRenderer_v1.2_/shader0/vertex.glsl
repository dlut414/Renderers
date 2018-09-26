/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Released under CC BY-NC
*/
#version 330 core

layout(location = 0) in vec3 vPos;

out vec4 mPos;
out float mvRadius;

uniform mat4    vMvp;
uniform mat4    vMv;
uniform mat4    fPro;
uniform float   height;
uniform float   radius;

void main()
{
    mPos = vec4(vPos, 1.f);

    vec4 mPt1 = mPos;
    vec4 mPt2 = mPt1 + vec4(0.f, 0.f, radius/2.f, 0.f);

    vec4 mvPt1 = vMv * mPt1;
    vec4 mvPt2 = vMv * mPt2;

    vec4 mvpPt1 = fPro * mvPt1;
    vec4 mvpPt2 = fPro * mvPt2;

    mvpPt1 = mvpPt1 / mvpPt1.w;
    mvpPt2 = mvpPt2 / mvpPt2.w;

    mvRadius = distance(vec3(mvPt1), vec3(mvPt2));

    gl_Position = vMvp * vec4(vPos, 1.0f);

    gl_PointSize = 2.f * height * distance(vec3(mvpPt1), vec3(mvpPt2));
}
