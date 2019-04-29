#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_TexPos;

uniform float u_Time;

void main()
{
	vec4 newColor = vec4(v_TexPos.xy, 0, 1);
//	float newTime = fract(u_Time);

	// Q »çÀÎ°î¼± ¾Æ·¡·Î Ä¥ÇØ¶ó
	float PI = 3.141592;
	float x = v_TexPos.x * 2 * PI + u_Time;
	float sinResult = (sin(x) + 1)/2;

	if(sinResult > v_TexPos.y && sinResult - 0.01 < v_TexPos.y) {
		newColor = vec4(1);
	}

	FragColor = newColor;
}
