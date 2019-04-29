#version 330

uniform float u_Time;
uniform sampler2D u_Texture;
in vec2 v_TexPos;

out vec4 FragColor;

void main()
{
	vec2 newTex = fract(v_TexPos * 2 + vec2(cos(u_Time), sin(u_Time)));
//	vec4 newColor = vec4(newTex, 0, 1); // 4등분한거 UV로 보이게~

	
	vec4 newColor = texture(u_Texture, newTex);
	

	FragColor = newColor;
}
