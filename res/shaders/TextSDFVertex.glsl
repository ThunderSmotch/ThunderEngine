// vertex shader
R"(
#version 450 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aCol;
layout(location = 2) in float aOutSize;
layout(location = 3) in vec4 aOutCol;

layout(location = 0) out vec2 texCoords;
layout(location = 1) out vec4 textCol;
layout(location = 2) out float outlineSize;
layout(location = 3) out vec4 outlineCol;

uniform mat4 u_ViewProjection;

void main()
{
    gl_Position = u_ViewProjection * vec4(aPos.xy, 0, 1);
    texCoords = aPos.zw;
    textCol = aCol;

    outlineSize = aOutSize;
    outlineCol = aOutCol;
}
)"