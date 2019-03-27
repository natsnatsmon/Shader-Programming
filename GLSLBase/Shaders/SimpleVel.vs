#version 330

in vec3 a_Position;
in vec3 a_Velocity;
in vec2 a_StartLifeTime;

uniform float u_Time;
uniform bool u_Repeat = true;

const vec3 c_Gravity = vec3(0, -9.8, 0);
void main()
{
	vec3 newPos = a_Position.xyz;
		
	float startTime = a_StartLifeTime.x;
	float lifeTime = a_StartLifeTime.y;

	float newTime = u_Time - startTime;

	if(newTime > 0) {
		float life = newTime;
		
		float remainingLife = lifeTime - life;

		if(u_Repeat == true) {
			remainingLife = 1.f;
			newTime = mod(newTime, lifeTime); // mod(나누기 전 값, 나눌 값)
		} 

		if(remainingLife < 0) {
			newPos = vec3(10000, 10000, 100000);
		}
		else {
			newPos += (a_Velocity * newTime) + (c_Gravity * 0.5 * newTime * newTime);
		}

	}
	else {
		newPos = vec3(10000, 10000, 100000);
	}

	
	gl_Position = vec4(newPos, 1);
}