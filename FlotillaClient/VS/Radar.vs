#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;

out vec4 posit;

void main()
{
	posit = vec4(pos, 1.0f);
	gl_Position = vec4(pos, 1.0f);
}