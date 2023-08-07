// vertex shader
R"(
#version 450 core

layout(location = 0) in vec3 aWorldPosition;
layout(location = 1) in vec3 aLocalPosition;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec4 aBorderColor;
layout(location = 4) in float aBorderThickness;
layout(location = 5) in float aThickness;
layout(location = 6) in float aFade;

uniform mat4 u_ViewProjection;

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	vec4 BorderColor;
	float BorderThickness;
	float Thickness;
	float Fade;
};

layout(location = 0) out VertexOutput Output;

void main()
{
	Output.LocalPosition = aLocalPosition;
	Output.Color = aColor;
	Output.BorderColor = aBorderColor;
	Output.BorderThickness = aBorderThickness;
	Output.Thickness = aThickness;
	Output.Fade = aFade;

	gl_Position = u_ViewProjection * vec4(aWorldPosition, 1.0);
}
)"