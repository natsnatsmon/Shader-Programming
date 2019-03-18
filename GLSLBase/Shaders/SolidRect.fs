#version 450

layout(location=0) out vec4 FragColor;

void main()
{
	FragColor = vec4(1,0,1,1);

	mat4 m = mat4(2.0);
	vec4 zVec = m[2];
	float yScale = m[1][1];
}
