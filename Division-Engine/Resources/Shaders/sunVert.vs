#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout(location = 10) uniform mat4 mvp;

out VertexShaderOut {
	vec2 TexCoords;
} vs_out;

void main() {
	vs_out.TexCoords = texCoords;
	gl_Position = mvp * vec4(position, 1.0);
}