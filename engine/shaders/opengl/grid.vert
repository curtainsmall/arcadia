#version 330 core

layout(location = 0) in vec3 i_pos;

uniform mat4 u_view_mat4;
uniform mat4 u_proj_mat4;
uniform float u_near_plane;
uniform float u_far_plane;

out vec3 v_near_point;
out vec3 v_far_point;
out mat4 v_view_mat4;
out mat4 v_proj_mat4;
out float v_near_plane;
out float v_far_plane;

vec3 unproject(float x,float y, float z, mat4 view, mat4 proj);

void main(){
    vec3 p = i_pos;
    v_near_point = unproject(p.x, p.y, 0.0, u_view_mat4, u_proj_mat4);
    v_far_point = unproject(p.x, p.y, 1.0, u_view_mat4, u_proj_mat4);
    v_view_mat4 = u_view_mat4;
    v_proj_mat4 = u_proj_mat4;
    v_near_plane = u_near_plane;
    v_far_plane = u_far_plane;
    gl_Position = vec4(i_pos, 1.0);
}

vec3 unproject(float x,float y, float z, mat4 view, mat4 proj){
    mat4 view_inv = inverse(view);
    mat4 proj_inv = inverse(proj);
    vec4 unprojected = view_inv * proj_inv * vec4(x, y, z, 1.0);
    return unprojected.xyz / unprojected.w;
}
