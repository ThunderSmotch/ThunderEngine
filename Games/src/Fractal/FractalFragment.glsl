// fragment shader
R"(
#version 330 core
in vec4 gl_FragCoord;

out vec4 FragColor;

uniform vec2 viewportDims;
uniform float zoom;
uniform vec2 center;

float maxIterations = 200;

vec2 squareImaginary(vec2 number){
	return vec2(
		pow(number.x,2)-pow(number.y,2),
		2*number.x*number.y
	);
}

float iterateMandelbrot(vec2 coord){
	vec2 z = vec2(0,0);
	for(int i=0; i < maxIterations; i++){
		z = squareImaginary(z) + coord;
		if( length(z) > 2) return i/maxIterations;
	}
	return 1.0f;
}

void main()
{	
	vec2 percentage = gl_FragCoord.xy / viewportDims;

	float monitorScale = viewportDims.x/viewportDims.y;

	vec2 bottomLeft = vec2(-monitorScale*1.0f, -1.0f)/zoom + center;
	vec2 topRight = vec2(monitorScale*1.0f, 1.0f)/zoom + center;

	vec2 pixelCoords = mix(bottomLeft, topRight, percentage);

	float shade = iterateMandelbrot(pixelCoords);
    FragColor = vec4(shade, shade, shade, 1.0);
}
)"