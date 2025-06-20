#version 430 core

layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec2 i_tex_coord;

out vec3 v_pivot;

uniform mat4 u_transform_mat = mat4(1.f);
uniform mat4 u_view_mat = mat4(1.f);
uniform mat4 u_proj_mat = mat4(1.f);

void main()
{
    gl_Position = u_proj_mat * u_view_mat * u_transform_mat * vec4(i_pos,1.0);
}
