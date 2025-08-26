#version 400 core
in vec2 wPos;

out vec4 FragColor;

uniform vec2 roots[3];
uniform int iterations;

const vec4 COLORS[3]  = vec4[3](
					vec4(0.901961, 0.901961, 0.45098, 1.0),
					vec4(0.45098, 0.901961, 0.901961, 1.0),
					vec4(0.901961, 0.45098, 0.901961, 1.0));

vec2 cmul(vec2 a, vec2 b)
{
	return vec2(a.x*b.x-a.y*b.y,a.x*b.y+a.y*b.x);
}

vec2 cdiv(vec2 a, vec2 b)
{
	return vec2(a.x*b.x+a.y*b.y, a.y*b.x-a.x*b.y)/(b.x*b.x+b.y*b.y);
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

uint MinIndex(float x, float y, float z)
{
   if (x <= y && x <= z) {
        return 0; // x is the minimum
    } else if (y <= x && y <= z) {
        return 1; // y is the minimum
    } else {
        return 2; // z is the minimum
    }
}

void main()
{
	vec2 z = wPos;
	int i;
	for(i = 0; i < iterations; i++)
	{
		vec2 fvalue = eval_func(z);
		z = z - cdiv(fvalue,eval_deriv(z));
		if(length(fvalue) < 0.0005) break;
	}
	uint idx = MinIndex(distance(z, roots[0]), distance(z, roots[1]), distance(z, roots[2]));
	FragColor = COLORS[idx];
	if(iterations >= 20) FragColor *= mix(1.0, 0.33, float(i)/float(iterations));
}
