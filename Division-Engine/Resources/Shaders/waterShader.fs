#version 450 core

in VertexShaderOut {
	vec2 texCoords;
} fs_in;

out vec4 fragColor;

layout(binding = 1) uniform sampler2D reflectionTexture;
layout(binding = 2) uniform sampler2D refractionTexture;

void main() {

	vec4 reflectColor = texture(reflectionTexture, fs_in.texCoords);
	vec4 refractColor = texture(refractionTexture, fs_in.texCoords);

	//fragColor = mix(reflectColor, refractColor, 0.5);
	fragColor = vec4(0.0, 0.0, 1.0, 1.0);
}