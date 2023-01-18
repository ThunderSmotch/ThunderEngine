#version 450 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec4 aCol;

layout(location = 0) out vec4 outCol;

uniform mat4 u_ViewProjection;

void main()
{
    gl_Position = u_ViewProjection * vec4(aPos.x, aPos.y, 0, 1);
    outCol = aCol;
}