#version 450 core

in VertexShaderOut {
	vec3 vertNormal;
	vec3 vertDir;
	vec2 texCoords;
} fs_in;

out vec4 fragColor;

uniform sampler2D tex2D;

void main() {
	vec4 texColor = texture(tex2D, fs_in.texCoords);
	fragColor = texColor;
}