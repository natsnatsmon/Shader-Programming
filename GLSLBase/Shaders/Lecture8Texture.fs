#version 330

in vec2 v_TexPos;
out vec4 FragColor;

uniform sampler2D u_TexSampler0;
uniform sampler2D u_TexSampler1;
uniform sampler2D u_TexSampler2;
uniform sampler2D u_TexSampler3;

void main()
{
	vec4 newColor;
	//vec2 newTex = vec2(v_TexPos.x, 1.0-v_TexPos.y);
	vec2 newTex = v_TexPos;

	if(v_TexPos.x < 0.5) {
		if(v_TexPos.y < 0.5) {
			newColor = texture(u_TexSampler2, vec2(newTex.x * 2, newTex.y * 2) );	
		}
		else {
			newColor = texture(u_TexSampler0, vec2(newTex.x * 2, fract(newTex.y * 2) ) );
		}
	} else {
		if(v_TexPos.y < 0.5 )
			newColor = texture(u_TexSampler3, vec2(fract(newTex.x * 2), newTex.y * 2) );
		else
			newColor = texture(u_TexSampler1, vec2(fract(newTex.x*2), fract(newTex.y*2) ) );
	}

	FragColor = vec4(newColor);
}
