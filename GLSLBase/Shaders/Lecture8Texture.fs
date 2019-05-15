#version 330

in vec2 v_TexPos;
out vec4 FragColor;

uniform float u_Time;
uniform sampler2D u_aniTexSampler;
uniform sampler2D u_TexSampler0;
uniform sampler2D u_TexSampler1;
uniform sampler2D u_TexSampler2;
uniform sampler2D u_TexSampler3;

void main()
{
	vec4 newColor;
	//vec2 newTex = vec2(v_TexPos.x, 1.0-v_TexPos.y);
	vec2 newTex = v_TexPos;

	newTex.y = newTex.y/6.0 + u_Time/6.0;
	
	newColor = texture(u_aniTexSampler, newTex);
	FragColor = vec4(newColor);
}
