#version 460

layout(push_constant) uniform PushConstants {
    mat4 mvp;
} pushConstants;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 outTexCoord;

void main()
{
    gl_Position = pushConstants.mvp * vec4(inPosition, 1.0f);

    gl_Position.y = -gl_Position.y;
    gl_Position.z = (gl_Position.z + gl_Position.y) / 2.0f;

    outTexCoord = inTexCoord;
}