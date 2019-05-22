#version 330

in float v_Grey;
layout(location=0) out vec4 FragColor;

void main()
{
	vec3 newColor = vec3(v_Grey);
	FragColor = vec4(newColor,1);
}
