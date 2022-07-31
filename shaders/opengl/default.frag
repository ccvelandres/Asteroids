#version 120

uniform sampler2D u_sampler2D;

varying vec2 v_uv;


void main()
{
    gl_FragColor = vec4(texture2D(u_sampler2D, v_uv).rgba);
} 