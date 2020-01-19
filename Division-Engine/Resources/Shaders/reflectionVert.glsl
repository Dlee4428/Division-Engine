#version 450 core
layout (location = 0) in vec4 vVertex;
layout (location = 1) in vec3 vNorm;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float time;

out vec3 vNormal;
out vec3 vPosition;

uniform sampler3D pNoise;

void main()
{
	vec3 noisePos = vec3(vVertex) + vNorm * texture(pNoise, vec3(vVertex) * time).xyz;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(noisePos, 1.0);

	vNormal = mat3(transpose(inverse(modelMatrix))) * vNorm;
	vPosition = vec3(modelMatrix * vec4(noisePos, 1.0));
}