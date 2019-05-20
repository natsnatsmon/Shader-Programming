#version 330

in vec2 v_TexPos;
out vec4 FragColor;

uniform float u_Time;

uniform float u_number;
uniform float u_resolX;
uniform float u_resolY;

uniform sampler2D u_animTexture;

void main()
{
	vec2 newTex;

	float x_index = floor(fract(u_number / u_resolX) * u_resolX);
	float y_index = floor(floor(u_number / u_resolY));

	newTex.x = v_TexPos.x/u_resolX + x_index/u_resolX;
	newTex.y = v_TexPos.y/u_resolY + y_index/u_resolY;
	newTex.x = u_resolX - newTex.x;

	vec4 newColor = texture(u_animTexture, newTex);

	FragColor = newColor;

}