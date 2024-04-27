// fragment shader
R"(
#version 450 core

layout(location = 0) in vec2 texCoords;
layout(location = 1) in vec4 textCol;
layout(location = 2) in float outlineSize;
layout(location = 3) in vec4 outlineCol;

layout(binding = 0) uniform sampler2D u_Texture;

out vec4 outColor;

void main()
{
    const float smoothing = 1.0f / (32.0f);

    float distance = texture(u_Texture, texCoords).r; // From 0 to 1

    float outlineFactor = smoothstep(0.5 - smoothing, 0.5, distance);
    //float outlineFactor = step(0.5 - smoothing, distance);
    vec4 color = mix(outlineCol, textCol, outlineFactor);

    float outlineDistance = 0.5f - outlineSize / 2.0f; // Convert outlineSize 0 to 1 to distance from 0.5 to 0.0

    float alpha = smoothstep(outlineDistance - smoothing, outlineDistance + smoothing, distance);


    if (alpha == 0.0f)
    {
        discard;
    }

    outColor = vec4(color.rgb, color.a * alpha);
}
)"