#version 330

in vec3 a_Position;
in vec3 a_Color;
in vec2 a_TexPos;

uniform float u_Time;
uniform float u_Color;

out vec2 v_TexPos;

void main()
{
	v_TexPos = a_TexPos;
	gl_Position = vec4(a_Position, 1);
}