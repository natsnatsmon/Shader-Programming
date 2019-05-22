#version 330

in vec3 a_Position;
out float v_Grey;

uniform float u_Time;
uniform float u_Color;
uniform float u_RandNum;

const float PI = 3.141592;

void main()
{
	vec3 newPos = a_Position;

	// x :: -0.5 ~ 0.5
	// y :: -0.5 ~ 0.5

	// weight :: 0 ~ 0.4
	float weightX = newPos.x + 0.5;
	float weightY = newPos.y + 0.5;

	// period :: 1 ~ 2.8 (곱하기는 주기가 몇번 돌게할건지)
	float periodX = 1.0 + (1.0 - weightY) * 0.5;
	float periodY = 1.0 + weightX * 0.5;

	// value :: 0 ~ 2PI
	float valueX = (weightX * 2 * PI * periodX) - u_Time;
	float valueY = (weightX * 2 * PI * periodY) - u_Time;
	
	// sinValue :: -1 ~ 1 * 0.5 = -0.5 ~ 0.5
	float sinValueX = sin(valueX) * 0.08;
	float sinValueY = sin(valueY) * 0.2;
	

	// x
	newPos.x = newPos.x - sinValueX * weightX;

	// y
	newPos.y = (newPos.y * (1.0-(weightX * 0.3))) + sinValueY * weightX;

	v_Grey = sinValueY + 0.5;


	gl_Position = vec4(newPos, 1);
}