#version 330

in vec3 a_Position;
out float v_Grey;

uniform float u_Time;
uniform float u_Color;

const float PI = 3.141592;

void main()
{
	vec3 newPos = a_Position;

	// x :: -0.5 ~ 0.5

	// weight :: 0 ~ 0.4
	float weight = (newPos.x + 0.5) * 0.4f;

	// period :: 1 ~ 2.8 (곱하기는 주기가 몇번 돌게할건지)
	float period = (1.0 + weight) * 2.0;

	// value :: 0 ~ 2PI
	float value = (newPos.x + 0.5) * 2 * PI * period - u_Time;
	
	// sinValue :: -1 ~ 1 * 0.5 = -0.5 ~ 0.5
	float sinValue = sin(value) * weight;
	
	newPos.x = newPos.x + (sinValue * weight);

	// y :: -0.5 ~ 0.5
	newPos.y = newPos.y + sinValue * weight;
	v_Grey = sinValue + 0.5;


	gl_Position = vec4(newPos, 1);
}