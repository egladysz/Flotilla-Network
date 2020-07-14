#version 330 core

out vec4 FragColor;

in vec4 posit;

uniform float x;
uniform float y;
uniform float radius;
uniform vec4 alpha;
uniform vec4 color;

uniform float screenWidth;
uniform float screenHeight;

void main()
{
	

	vec2 core = (vec2(x/screenWidth,y/screenHeight))*2;//-vec2(0.5f,0.5f))*2;
	vec2 curPos = vec2(posit.x,posit.y);
	vec2 span = curPos-core;
	span.x *= screenWidth;
	span.y *= screenHeight;
	float dist = sqrt(dot(span,span));

	vec4 fc = vec4(0.0f,0.0f,0.0f,0.5f);
	if(dist <= radius)
	{
		fc = vec4(color.r*alpha.r,color.g*alpha.g,color.b*alpha.b,color.a*alpha.a);
	}
    FragColor = fc;
} 