#version 400 core
layout (location = 0) in vec2 vPos;

out vec2 wPos;

uniform vec3 viewport;
uniform vec2 offset;

void main()
{
	wPos = vPos;
	float aspect = viewport.x/viewport.y;
	wPos.x += sign(wPos.x) * (aspect / 2.0);
	wPos /= viewport.z; //handle zooming
	wPos += offset; //handle panning
	gl_Position = vec4(vPos, 0.0, 1.0);
}
