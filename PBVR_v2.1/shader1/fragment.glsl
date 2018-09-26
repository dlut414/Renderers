/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Released under CC BY-NC
*/
#version 330 core

in vec3 viewVertexNorm;
in float vertColor;

out vec4 color;

void main()
{
    vec2 dis = gl_PointCoord.st * vec2(2.f, 2.f) - vec2(1.f, 1.f);
    if(dot(dis, dis) > 1.f) discard;
    //const vec4 _lightColor = vec4( 0.0f, 0.5f, 0.0f, f_alpha_color );
    const vec3 _lightDir = normalize( vec3(3.0f, 3.0f, 3.0f) );
    const vec3 _eyeDir = normalize( vec3(0.0f, 0.0f, 1.0f) );
    const vec3 _materialColor = vec3( 0.3f, 0.7f, 0.1f );
    const vec3 _materialAmbientColor = vec3( 0.3f, 0.3f, 0.3f ) * _materialColor;
    //const vec3 _materialSpecularColor = vec3( 0.3f, 0.3f, 0.3f );

    vec3  _n = normalize( viewVertexNorm );

    //clamp(dot(_n, _lightDir), 0, 1);
    float _cosTheta = max( 0.0f, dot(_n, _lightDir) );
    float _cosBeta = max( 0.0f, dot(reflect(-_lightDir, _n), _eyeDir) );

    //color = vec4( _materialAmbientColor + (_cosTheta * _materialColor) + (pow(_cosBeta, 5) * _materialColor), 1.f );
    //color = vec4( _materialAmbientColor + (_cosTheta * _materialColor), 1.f );
    //color = vec4( (1.f-vertColor)*vec3(0.f, 0.f, 1.f) + vertColor*vec3(1.f, 0.f, 0.f), 1.f);

    float maxVel = 6.f;
    float midVel = maxVel / 2.f;

    if(vertColor < midVel) color = vec4( 2.f* (midVel-vertColor) * vec3(0.f, 0.f, 1.f) + 2.f* vertColor         *vec3(0.f, 1.f, 0.f), 1.f);
    else                   color = vec4( 2.f* (maxVel-vertColor) * vec3(0.f, 1.f, 0.f) + 2.f* (vertColor-midVel)*vec3(1.f, 0.f, 0.f), 1.f);

}

