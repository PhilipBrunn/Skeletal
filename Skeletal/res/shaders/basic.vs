#version 330 core
layout(location = 0) in vec3 pos;
layout(location= 1)  in vec3 color;
layout(location = 2) in vec2 texCoord;

out vec3 v_color;
out vec2 v_texCoord;

uniform mat4 u_transform;

void main()
{
    gl_Position = u_transform * vec4(pos.x, pos.y, pos.z, 1.0);
	v_color = color;
	v_texCoord = texCoord;
}