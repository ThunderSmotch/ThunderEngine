#version 450 core


struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	vec4 BorderColor;
	float BorderThickness;
	float Thickness;
	float Fade;
};

layout(location = 0) in VertexOutput Input;

out vec4 oColor;

void main()
{
	// Calculate distance and fill circle with white

	// Distance is 0 in the border and 1 in the center
	float distance = 1.0 - length(Input.LocalPosition);
	float circle = smoothstep(0.0, Input.Fade, distance);
	circle *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

	// float

	if (circle == 0.0)
		discard;

	// Set output color
	float border = smoothstep(Input.BorderThickness, Input.BorderThickness + Input.Fade, distance);
	oColor = mix(Input.BorderColor, Input.Color, border);
	oColor.a *= circle;
}