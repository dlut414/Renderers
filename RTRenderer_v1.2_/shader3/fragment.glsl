/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Released under CC BY-NC
*/
#version 330 core

in vec3 fPos;

out vec4 color;

void main()
{
    color = vec4(normalize(fPos), 1.f);
}
