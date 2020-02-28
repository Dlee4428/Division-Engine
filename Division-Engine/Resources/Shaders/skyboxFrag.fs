#version 450 core

in VertexShaderOut {
	vec3 texCoords;
} fs_in;

out vec4 fragColor;

uniform samplerCube cubeMap;

void main() {
	vec4 envColor = texture(cubeMap, fs_in.texCoords);
	float lowerLimit = 0.3;
	float upperLimit = 0.1;
	vec3 skyColor = vec3(0.6, 0.6, 0.6);
	float factor = (fs_in.texCoords.y - lowerLimit) / (upperLimit - lowerLimit);
	factor = clamp(factor, 0.0, 1.0);

	fragColor = mix(vec4(skyColor, 1.0), envColor, factor);
}