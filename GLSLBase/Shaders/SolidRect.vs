#version 330

in vec4 a_Position;
in vec3 a_Color;

uniform float u_Time;

void main()
{
	vec3 temp;

	// 원의 방정식 - (a, b) = 원의 중심, r = 반지름
	// x = a + rcos(), y = b + rsin()

	float newX, newY;
	newX = sin(u_Time * 3.141592 * 2);
	newY = cos(u_Time * 3.141592 * 2);

	newX += a_Position.x;
	newY += a_Position.y;

	// float r = 0.5f;
	// a_Position.x = r * sin(90);
	// a_Position.y = r * cos(30);
	
//	temp = a_Position.xyz * u_Time;
	
	gl_Position = vec4(newX, newY, 0, 1);
//	gl_Color = vec4(a_Color.xyz, 1);
}