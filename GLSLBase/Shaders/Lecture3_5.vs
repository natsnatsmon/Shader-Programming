#version 330

in vec3 a_Position;
in vec3 a_Velocity;
in vec4 a_Time;

uniform float u_Time;
uniform bool u_Repeat = true;

const float pi = 3.141592;
const mat3 c_RParticle = mat3(0, -1, 0, 1, 0, 0, 0, 0, 0);

void main()
{
	vec3 newPos = a_Position.xyz;


	float startTime = a_Time.x;
	float lifeTime = a_Time.y;
	float ratio = a_Time.z;
	float amp = a_Time.w;

	float newTime = u_Time - startTime;

	if(newTime > 0) {

		amp = amp * newTime * newTime;

		newTime = mod(newTime, lifeTime);
		newPos = newPos + a_Velocity * newTime;

		vec3 vSin = a_Velocity * c_RParticle;

		newPos = newPos + vSin * sin(newTime * pi * 2 * ratio) * amp;
	} 
	else {
		newPos = vec3(10000, 10000, 10000);
	}

	gl_Position = vec4(newPos.xyz, 1);
}

// 진폭, 속도, 주기?