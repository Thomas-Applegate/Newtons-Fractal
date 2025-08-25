#version 400 core
in vec2 wPos;

out vec4 FragColor;

uniform vec2 roots[3];

void main()
{
	FragColor = vec4(fract(wPos), 0.0, 1.0);
}
