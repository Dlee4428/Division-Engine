#version 450 core

out VertexShaderOut {
	vec2 texCoords;
} vs_out;

layout (location = 0) in vec4 vVertex;
layout (location = 1) in vec2 vTexCoords;
layout (location = 2) in vec3 vNormal;

layout (location = 10) uniform mat4 projMat;
layout (location = 11) uniform mat4 viewMat;
layout (location = 12) uniform mat4 modelMat;
layout (location = 13) uniform mat3 normalMat;

void main() {
	gl_Position = projMat * viewMat * modelMat * vVertex;
	vs_out.texCoords = vTexCoords;
}