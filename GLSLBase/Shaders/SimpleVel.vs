#version 330

in vec3 a_Position;
in vec3 a_Velocity;
in vec3 a_Color;

uniform float u_Time;

void main()
{
	vec3 newPos = a_Position.xyz;
	
	float newTime = fract(u_Time); // fract�� ���ĸ� �Ҽ����� ���°�.. �̷����ϸ� 0~1�� �ݺ��Ѵ�
	newPos += a_Velocity * newTime;

	gl_Position = vec4(newPos, 1);
}