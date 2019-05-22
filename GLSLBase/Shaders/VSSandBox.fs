#version 330

in float v_Grey;
in vec2 v_Tex;

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

void main()
{
	vec3 newColor = vec3(v_Grey);
	FragColor = texture(u_Texture, v_Tex);
}
