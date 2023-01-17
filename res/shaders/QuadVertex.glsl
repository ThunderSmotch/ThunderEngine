#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aCol;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aTexIndex;

layout(location = 0) out vec4 outCol;
layout(location = 1) out vec2 outTexCoord;
layout(location = 2) out float outTexIndex;

uniform mat4 u_ViewProjection;

void main()
{
    gl_Position = u_ViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);

    outCol = aCol;
    outTexCoord = aTexCoord;
    outTexIndex = aTexIndex;
}