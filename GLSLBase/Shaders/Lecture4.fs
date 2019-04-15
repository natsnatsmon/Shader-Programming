#version 330

layout(location=0) out vec4 FragColor;

in vec3 v_Position;
in vec2 v_UV;

uniform float u_Time;
uniform vec2 u_EnemyPoints[5];
//const vec2 c_EnemyPoints[5] = {vec2(0.f, 0.f), vec2(0.5f, 0.5f), vec2(0.25f, -0.25f), vec2(-0.2f, -0.3f), vec2(0.3f, -0.4f)};

void main()
{
//	vec3 newColor;
//	newColor.r = v_Position.x;
//	newColor.g = v_Position.y;
//	newColor.b = 0;

	vec2 newUV = v_UV - vec2(0.5, 0.5); // -0.5 ~ 0.5 사이의 값을 가진다.

	float enemyPoint = 0;

	float distance = length(newUV);
	float newTime = fract(u_Time);
	float ringWidth = 0.05;

	
	if(distance < newTime + ringWidth && distance > newTime) {
		float temp = (distance - newTime) / ringWidth;
		enemyPoint = temp;

		for(int i = 0; i < 5; ++i) {
			vec2 newPoint = u_EnemyPoints[i];
			vec2 newVec = newPoint - newUV; // 적 위치 - 내 위치
			float distance = length(newVec);

			if(distance < 0.01) {
				enemyPoint += 0.5f;
			}
		}

	}
	
	FragColor = vec4(enemyPoint);
} 