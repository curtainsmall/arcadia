#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_tex_coord;

out vec3 v_normal;
out vec2 v_tex_coord;

uniform mat4 u_model_mat = mat4(1.f);
uniform mat4 u_camera_mat = mat4(1.f);

uniform mat3 u_normal_mat = mat3(1.f);

void main()
{
    v_normal = u_normal_mat * a_normal;
    v_tex_coord =  a_tex_coord;
    gl_Position = u_camera_mat * u_model_mat * vec4(a_pos, 1.0);
}