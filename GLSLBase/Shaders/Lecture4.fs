#version 330

layout(location=0) out vec4 FragColor;

in vec3 v_Position;
in vec2 v_UV;

void main()
{
	vec3 newColor;
	newColor.r = v_Position.x;
	newColor.g = v_Position.y;
	newColor.b = 0;

	float distance = sqrt(v_Position.x * v_Position.x + v_Position.y * v_Position.y);

	if(distance < 0.5f && distance >= 0.49f) {
		FragColor = vec4(newColor.rgb, 1);
	
	}
	else {
			FragColor = vec4(0, 0, 0, 0);

	}
}
