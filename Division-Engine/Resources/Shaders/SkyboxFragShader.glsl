#version 450 core

out vec4 FragColor;

in vec3 texture_pos;

uniform samplerCube u_texture;

void main()
{    
    FragColor = texture(u_texture, texture_pos);
}