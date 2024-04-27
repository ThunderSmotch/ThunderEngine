// fragment shader
R"(
#version 450 core

layout(location = 0) in vec4 aCol;

out vec4 outColor;

void main()
{
    outColor = aCol;
}
)"