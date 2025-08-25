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

vec2 eval_func(vec2 z)
{
	return cmul(cmul(z-roots[0], z-roots[1]), z-roots[2]);
}

vec2 eval_deriv(vec2 z)
{
	return cmul(roots[0], roots[1]) + cmul(roots[2], roots[0]+roots[1])
		+ cmul(z, -2*roots[0]-2*roots[1]-2*roots[2]+3*z);
}

void main()
{
	FragColor = vec4(fract(wPos), 0.0, 1.0);
}
