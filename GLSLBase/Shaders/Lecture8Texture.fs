#version 330

uniform float u_Time;
uniform sampler2D u_Texture;
in vec2 v_TexPos;

out vec4 FragColor;

void main()
{
	vec2 newTex = v_TexPos;
	vec4 newColor = vec4(newTex, 0, 1);
	
	// 1��
	newTex.x = v_TexPos.x;
	newTex.y = abs(1-(v_TexPos.y * 2.0));


	// 2��
	newTex.x = fract(v_TexPos.x * 3.0);
	newTex.y = v_TexPos.y * 0.33;
	
	//newTex.y += floor(v_TexPos.x * 3.0) * 0.33;


	// 3��
	newTex.x = fract(v_TexPos.x * 3.0);
	newTex.y = v_TexPos.y * 0.33;
	
	newTex.y += floor((1 - v_TexPos.x) * 3.0) * 0.33;
	newTex.y = abs(newTex.y);


	// 4��
	newTex.x = v_TexPos.x;
	newTex.y = (2-floor(v_TexPos.y * 3.0))/3; // 2, 1, 0�� �ȴ�.. /3�� �ؼ� 2/3, 1/3, 0�� �Ǿ���..
	newTex.y += fract(v_TexPos.y * 3.0)/3;

	
	// 5�� ���� ����� ���迡 �����̴� . ... ... ...
	// ��� ����µ� .. ��ü .. x�� 0.25 ���ϰ� y�� 2 ���ؼ� floor ���ϸ� �ȴٰ�..?
	newTex.x = fract((v_TexPos.x+0.25) * 2);
	newTex.y = fract(v_TexPos.y * 2);

	
	newColor = texture(u_Texture, newTex);

	FragColor = newColor;
}
