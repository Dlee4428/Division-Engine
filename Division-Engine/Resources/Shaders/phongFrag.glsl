#version 450 core

#define NUM_LIGHTS 2

in  vec3 vertNormal;
in  vec3 lightDir[NUM_LIGHTS];
in  vec3 vertDir;
out vec4 fragColor; /// color of 1 pixel

vec4 ks[NUM_LIGHTS];
vec4 kd[NUM_LIGHTS];
vec4 ka[NUM_LIGHTS];

float diff[NUM_LIGHTS];
vec3 reflection[NUM_LIGHTS];
float spec[NUM_LIGHTS];

void main() { 
	ks[0] = vec4(0.3, 0.3, 0.3, 0.0);
	kd[0] = vec4(0.8, 0.0, 0.9, 0.0);
	ka[0] = 0.1 * kd[0];

	ks[1] = vec4(0.2, 0.2, 0.2, 0.0);
	kd[1] = vec4(0.9, 0.3, 0.0, 0.0); 
	ka[1] = 0.1 * kd[1];

	for(int i = 0; i < NUM_LIGHTS; i++){
		diff[i] = max(dot(vertNormal, lightDir[i]), 0.0); /// max represent the biggest number of two arguments
		reflection[i] = normalize(reflect(lightDir[i], vertNormal));
		spec[i] = max(dot(vertDir, reflection[i]), 0.0);

		if(diff[i] > 0.0){
			spec[i] = pow(spec[i], 10.0); /// raise the power of spectacular
		}

		fragColor +=  ka[i] + (diff[i] * kd[i]) + (spec[i] * ks[i]);
	}
} 

