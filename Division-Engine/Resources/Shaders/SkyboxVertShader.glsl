#version 450 core

layout (location = 0) in vec3 pos;

out vec3 texture_pos;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
    texture_pos = vec3(-pos.x, pos.y, pos.z);
    gl_Position = u_projection * u_view * vec4(pos, 1.0);
}  