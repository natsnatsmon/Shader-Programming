#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

in vec4 v_OriXY;

void main()
{
	FragColor = vec4(v_OriXY, 0, 1);
}
