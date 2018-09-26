/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Published under CC BY-NC
*/
#version 330 core

in vec3 viewVertexNorm;

out vec4 color;

void main()
{
    //const vec4 _lightColor = vec4( 0.0f, 0.5f, 0.0f, f_alpha_color );
    const vec3 _lightDir = normalize( vec3(3.0f, 3.0f, 3.0f) );
    const vec3 _eyeDir = normalize( vec3(0.0f, 0.0f, 1.0f) );
    const vec3 _materialColor = vec3( 0.1f, 0.7f, 0.3f );
    const vec3 _materialAmbientColor = vec3( 0.3f, 0.3f, 0.3f ) * _materialColor;
    //const vec3 _materialSpecularColor = vec3( 0.3f, 0.3f, 0.3f );

    vec3  _n = normalize( viewVertexNorm );

    //clamp(dot(_n, _lightDir), 0, 1);
    float _cosTheta = max( 0.0f, dot(_n, _lightDir) );
    float _cosBeta = max( 0.0f, dot(reflect(-_lightDir, _n), _eyeDir) );

    color = vec4( _materialAmbientColor + (_cosTheta * _materialColor) + (pow(_cosBeta, 5) * _materialColor), 1.f );
    //color = vec4( _materialAmbientColor + (_cosTheta * _materialColor), 1.f );
    //color = vec4(_materialColor, 1.f);
}
