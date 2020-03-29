#version 450 core

layout(location = 0) in vec3 aPos;

layout (location = 10) uniform mat4 projMat;
layout (location = 11) uniform mat4 viewMat;
layout (location = 12) uniform mat4 modelMat;

out VertexShaderOut {
	vec3 texCoords;
} vs_out;


void main() {
	vs_out.texCoords = vec3(aPos.x, -aPos.y, aPos.z);
	gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);
}