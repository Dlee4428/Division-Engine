#version 450 core

uniform sampler2D tex2D;
out vec4 color;

in VertexShaderOut {
	vec2 TexCoords;
} fs_in;

void main() {
	// color = vec4(1.0, 1.0, 1.0, 1.0);

	color = texture(tex2D, fs_in.TexCoords);
}