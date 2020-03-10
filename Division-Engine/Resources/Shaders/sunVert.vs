#version 450 core

out VertexShaderOut {
	vec3 vertNormal;
	vec3 vertDir;
	vec2 texCoords;
} vs_out;

layout (location = 0) in vec4 vVertex;
layout (location = 1) in vec4 vNormal;
layout (location = 2) in vec2 texCoords;

layout (location = 10) uniform mat4 projMat;
layout (location = 11) uniform mat4 viewMat;
layout (location = 12) uniform mat4 modelMat;
layout (location = 13) uniform mat3 normalMat;

void main() {
	vs_out.texCoords = texCoords;
	vs_out.vertNormal = mat3(transpose(inverse(normalMat))) * vNormal.xyz; // Rotate the normal to the correct orientation 
	
	vec3 vertPos = vec3(viewMat * modelMat * vVertex); //Creates the vertex Position(-eyedir);
	vs_out.vertDir = normalize(vertPos);
	
	gl_Position = projMat * viewMat * modelMat * vVertex;
}