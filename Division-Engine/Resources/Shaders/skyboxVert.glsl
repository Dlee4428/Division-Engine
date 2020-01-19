#version 450 core
layout (location = 0) in vec4 vVertex;

out vec3 texCoords;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

void main()
{
    texCoords = vec3(vVertex) * vec3(-1.0, 1.0, 1.0);
    gl_Position = projectionMatrix * modelMatrix * vVertex;
}  