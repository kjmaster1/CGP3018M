#version 440 core
out vec4 colour;
uniform float uTime;

void main()
{
	const float timespan = 10000.0;
	float w = sin(2.0 * 3.14159 * uTime / timespan) * 0.5 + 0.5;
	colour = vec4(1.0f, 0.3f * w, 0.0f * w, 1.0f);
}