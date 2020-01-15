#version 450
layout (location = 0) in vec4 vVertex;

out vec3 texCoords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
    texCoords = vec3(vVertex) * vec3(-1.0, 1.0, 1.0);
    gl_Position = proj * view * model * vVertex;
}  