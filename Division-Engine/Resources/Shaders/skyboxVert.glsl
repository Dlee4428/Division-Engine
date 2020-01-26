#version 450 core
layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
    texCoords = aPos * vec3(-1.0, 1.0, 1.0);
    gl_Position = projectionMatrix * viewMatrix * vec4(aPos, 1.0);
}  