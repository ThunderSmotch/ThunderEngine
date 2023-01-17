#version 450 core

layout(location = 0) in vec2 texCoords;

layout (binding = 0) uniform sampler2D u_Texture;

out vec4 outColor;

void main()
{
    vec4 texColor = vec4(1,1,1, texture(u_Texture, texCoords).r);

    outColor = texColor;
}