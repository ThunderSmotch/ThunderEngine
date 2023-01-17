#version 450 core

layout(location = 0) in vec4 aPos;

layout(location = 0) out vec2 texCoords;

uniform mat4 u_ViewProjection;

void main()
{
    gl_Position = u_ViewProjection * vec4(aPos.xy, 0, 1);
    texCoords = aPos.zw;
}