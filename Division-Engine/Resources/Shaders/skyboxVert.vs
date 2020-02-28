#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 3) uniform mat4 mvp;

out VertexShaderOut {
	vec3 texCoords;
} vs_out;


void main() {
	vs_out.texCoords = vec3(aPos.x, -aPos.y, aPos.z);
	gl_Position = mvp * vec4(aPos, 1.0);
}