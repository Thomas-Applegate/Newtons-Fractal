#version 400 core
in vec2 wPos;

out vec4 FragColor;

uniform vec2 roots[3];

void main()
{
	bool in_function = false;
	float line_width = 0.02;
	float xDist = fract(wPos.x);
	float yDist = fract(wPos.y);
	if(xDist < line_width || xDist > (1.0-line_width)) { in_function = true; }
	if(yDist < line_width || yDist > (1.0-line_width)) { in_function = true; }
	FragColor = mix(vec4(0.29f, 0.87f, 0.98f, 1.0f),vec4(0.0f, 0.0f, 0.0f, 1.0f),in_function);
	//FragColor = vec4(wPos, 0.0, 1.0);
}
