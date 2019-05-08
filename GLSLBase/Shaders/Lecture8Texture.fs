#version 330

uniform float u_Time;
uniform sampler2D u_Texture;
in vec2 v_TexPos;

out vec4 FragColor;

void main()
{
	vec2 newTex = v_TexPos;
	vec4 newColor = vec4(newTex, 0, 1);
	
	// 1번
	newTex.x = v_TexPos.x;
	newTex.y = abs(1-(v_TexPos.y * 2.0));


	// 2번
	newTex.x = fract(v_TexPos.x * 3.0);
	newTex.y = v_TexPos.y * 0.33;
	
	//newTex.y += floor(v_TexPos.x * 3.0) * 0.33;


	// 3번
	newTex.x = fract(v_TexPos.x * 3.0);
	newTex.y = v_TexPos.y * 0.33;
	
	newTex.y += floor((1 - v_TexPos.x) * 3.0) * 0.33;
	newTex.y = abs(newTex.y);


	// 4번
	newTex.x = v_TexPos.x;
	newTex.y = (2-floor(v_TexPos.y * 3.0))/3; // 2, 1, 0이 된다.. /3을 해서 2/3, 1/3, 0이 되었다..
	newTex.y += fract(v_TexPos.y * 3.0)/3;

	
	// 5번 벽돌 모양은 시험에 낼것이다 . ... ... ...
	// 어떻게 만드는데 .. 대체 .. x는 0.25 더하고 y는 2 곱해서 floor 취하면 된다고..?
	newTex.x = fract((v_TexPos.x+0.25) * 2);
	newTex.y = fract(v_TexPos.y * 2);

	
	newColor = texture(u_Texture, newTex);

	FragColor = newColor;
}
