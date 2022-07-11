#version 120

uniform mat4 mvp;

attribute vec3 vPos;

varying vec4 fragColor;

void main()
{
    gl_Position = mvp * vec4(vPos, 1.0);
    fragColor = vec4(1.0);
}