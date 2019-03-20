#version 450

layout(location = 0) in vec4 a_Position
layout(location = 1) in vec3 a_Color

uniform float u_Time;

void main()
{
	vec3 temp;
	temp = a_Position.xyz * a_Position.w * u_Time;

	gl_Position = vec4(temp, 1);
	//gl_Position = vec4(a_Position.xyz * 0.5, 1);
	gl_Color = vec4(a_Color, 1);
}