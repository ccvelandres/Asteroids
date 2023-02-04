#version 120

/** @todo: needs work to support sprite sheets */

uniform mat4 u_mvp;

attribute vec3 a_v;
attribute vec3 a_vn;
attribute vec2 a_vt;

varying vec2 v_uv;

void main()
{
    gl_Position = u_mvp * vec4(a_v, 1.0);
    v_uv = a_vt;
}