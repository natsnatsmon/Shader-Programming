#version 330

layout(location=0) out vec4 FragColor;

in vec3 v_Position;
in vec2 v_UV;

void main()
{
	vec3 newColor;
	newColor.r = v_Position.x;
	newColor.g = v_Position.y;
	newColor.b = 0;

	float distance = sqrt(v_Position.x * v_Position.x + v_Position.y * v_Position.y);


	vec2 newUV = v_UV - vec2(0.5, 0.5);
	float grey = sin(distance * 3.141592 * 4 * 4); // 거리, PI, ?, 주기

	FragColor = vec4(grey);

} 