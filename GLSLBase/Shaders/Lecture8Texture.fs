#version 330

in vec2 v_TexPos;
out vec4 FragColor;

uniform float u_Time;

uniform int u_number[3]; // 3 digits
uniform sampler2D u_aniTexSampler;
void main()
{
	vec4 newColor;
	int newIndex = int(floor(v_TexPos.x*3.0));
	// float newy = 9.0 - float(u_number[1]);
	vec2 newTex = v_TexPos;
	float newy = 5.0 - float(u_number[newIndex]);

	newTex.x = fract(newTex.x * 3);
	newTex.y = newTex.y/6.0 + newy/6.0;
	
	newColor = texture(u_aniTexSampler, newTex);
	FragColor = vec4(newColor);
}
