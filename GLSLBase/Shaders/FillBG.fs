#version 330

layout(location=0) out vec4 FragColor;

uniform float u_Alpha;

void main()
{
	FragColor = vec4(0,0,0,u_Alpha);
}
