/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Released under CC BY-NC
*/
#version 330 core

in vec4 mPos;
in float mvRadius;

out vec4 color;

uniform mat4    vMv;
uniform mat4    fMvpInv;
uniform mat4    fPro;

void main()
{
    vec3 pixelPos;
    pixelPos.xy = gl_PointCoord.st * vec2(2.f, -2.f) - vec2(1.f, -1.f);

    float mag = length(pixelPos.xy);
    if(mag >= 1.f) discard;

    pixelPos.z = sqrt(1.f - mag);

    vec4 mvPixel = vMv * mPos + vec4(mvRadius * pixelPos, 0.f);

    vec4 mvpPixel = fPro * mvPixel;

    float ndcDepth = mvpPixel.z/mvpPixel.w;

    gl_FragDepth = ndcDepth * 0.5f + 0.5f;

    color = fMvpInv * mvpPixel;
    //color.xyz = normalize(color.xyz);
}
