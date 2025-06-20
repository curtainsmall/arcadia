#version 330 core
layout(location = 0 ) in vec3 a_pos;

out vec3 v_tex_coord;

uniform mat4 u_proj_mat = mat4(1.f);
uniform mat4 u_view_mat = mat4(1.f);

void main(){
    v_tex_coord = a_pos;
    gl_Position = (u_proj_mat * u_view_mat * vec4(a_pos,1.f)).xyww;
    
}