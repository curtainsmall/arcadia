#version 430 core

in vec3 v_pivot;

out vec4 o_color;

uniform vec3 u_color;

void main()
{
    o_color = vec4(u_color,1);
}