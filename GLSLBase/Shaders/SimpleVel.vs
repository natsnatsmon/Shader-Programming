#version 330

in vec3 a_Position;
in vec3 a_Velocity;
in vec3 a_Color;

uniform float u_Time;

void main()
{
	vec3 newPos = a_Position.xyz;
	
	float newTime = fract(u_Time); // fract가 뭐냐면 소수점을 떼는거.. 이렇게하면 0~1만 반복한다
	newPos += a_Velocity * newTime;

	gl_Position = vec4(newPos, 1);
}