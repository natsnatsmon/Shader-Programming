#version 330

in vec3 a_Position;
in vec3 a_Color;

uniform float u_Time;
uniform float u_Color;

const float PI = 3.141592;

void main()
{
	vec3 newPos = a_Position;

	// x :: -0.5 ~ 0.5
	float value = (newPos.x + 0.5) * 2 * PI + u_Time;
	float sinValue = sin(value) * 0.5;

	// y :: -0.5 ~ 0.5
	newPos.y = newPos.y + sinValue;

	gl_Position = vec4(newPos, 1);
}