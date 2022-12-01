#version 120

uniform vec4 u_tile = vec4(0.0f, 0.0f, 1.0f, 1.0f);
uniform sampler2D u_sampler2D;

varying vec2 v_uv;

void main()
{
    float scaled_u = u_tile.x + ((u_tile.z - u_tile.x) * v_uv.x);
    float scaled_v = u_tile.y + ((u_tile.w - u_tile.y) * v_uv.y);
    gl_FragColor = vec4(texture2D(u_sampler2D, vec2(scaled_u, scaled_v)).rgba);
}