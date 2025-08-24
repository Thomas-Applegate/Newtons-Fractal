#version 400 core
layout (location = 0) in vec2 vPos;

out vec2 wPos;

uniform vec2 scale;
uniform vec2 offset;

void main()
{
	wPos = vPos;
	wPos.x += sign(wPos.x) * (scale.x / 2.0);
	wPos *= scale.y;
	wPos += offset;
	gl_Position = vec4(vPos, 0.0, 1.0);
}
