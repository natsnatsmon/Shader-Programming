#version 330

uniform float u_Time;
uniform sampler2D u_Texture;
in vec2 v_TexPos;

out vec4 FragColor;

void main()
{
	vec2 newTex;
	vec4 newColor = vec4(newTex, 0, 1);
	newTex.x = v_TexPos.x;

	newTex.y = fract(v_TexPos.y * 2);
	newTex.y = abs(newTex.y - v_TexPos.y);


	// 한 줄로 끝낼 수 있다.. . ...
	// newTex = abs(v_Tex - vec2(0, 0.5)) + vec2(0, 0.5);




	newColor = texture(u_Texture, newTex);

	FragColor = newColor;
}
