#version 330

in vec3 a_Position;
in vec2 a_UV;

out vec3 v_Position;
out vec2 v_UV;

void main()
{
	gl_Position = vec4(a_Position, 1);
	v_Position = a_Position;
	v_UV = a_UV;
}
