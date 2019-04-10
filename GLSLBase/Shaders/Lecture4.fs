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


	// UV
	FragColor = vec4(v_UV.xy, 0, 1);
	FragColor = vec4(v_Position.xy + vec2(0.5, 0.5), 0, 1); // 둘 다 같은 결과
}
