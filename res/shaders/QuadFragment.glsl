#version 450 core

layout(location = 0) in vec4 aCol;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in float aTexIndex;

layout (binding = 0) uniform sampler2D u_Textures[32];

out vec4 outColor;

void main()
{
    vec4 texColor = aCol*texture(u_Textures[int(aTexIndex)], aTexCoord);

    if (texColor.a == 0.0)
        discard;

    outColor = texColor;
}