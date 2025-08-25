#version 400 core
in vec2 wPos;

out vec4 FragColor;

uniform vec2 roots[3];

vec2 cconj(vec2 a)
{
	return vec2(a.x, -a.y);
}

vec2 cmul(vec2 a, vec2 b)
{
	return vec2(a.x*b.x-a.y*b.y,a.x*b.y+a.y*b.x);
}

vec2 cdiv(vec2 a, vec2 b)
{
	vec2 ret = vec2(a.x*b.x+a.y*b.y, a.y*b.x-a.x*b.y);
	return ret/(a.y*a.y+b.y*b.y);
}

vec2 csquared(vec2 a)
{
	return cmul(a,a);
}

vec2 ccubed(vec2 a)
{
	return cmul(a, csquared(a));
}

void main()
{
	FragColor = vec4(fract(wPos), 0.0, 1.0);
}
