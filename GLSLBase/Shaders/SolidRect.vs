#version 330

in vec3 a_Position;
in vec3 a_Color;

uniform float u_Time;
uniform float u_Color;

void main()
{
	gl_Position = vec4(a_Position, 1);
}