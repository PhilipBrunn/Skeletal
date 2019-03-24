#version 330 core

out vec4 f_color;

in vec3 v_color;
in vec2 v_texCoord;

uniform sampler2D u_texture;
uniform sampler2D u_texture1;


void main()
{
    f_color = mix(texture(u_texture, v_texCoord), texture(u_texture1, 1.0-v_texCoord), 0.5);
}