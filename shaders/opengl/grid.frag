#version 330 core

in vec3 v_near_point;
in vec3 v_far_point;
in mat4 v_view_mat4;
in mat4 v_proj_mat4;
in float v_near_plane;
in float v_far_plane;

out vec4 o_color;

vec4 grid(vec3 frag_pos, float scale, bool draw_axis);
float compute_depth(vec3 pos);
float compute_linear_depth(vec3 pos);

void main(){
    float t = -v_near_point.y / (v_far_point.y - v_near_point.y);
    vec3 frag_pos = v_near_point + t * (v_far_point - v_near_point);

    gl_FragDepth = compute_depth(frag_pos);

    float linear_depth = compute_linear_depth(frag_pos);
    float fading = max(0, 0.5 - linear_depth);
    o_color = (grid(frag_pos,10,true) + grid(frag_pos,1,true)) * float(t > 0);

    o_color.a *= fading;
}

vec4 grid(vec3 frag_pos, float scale, bool draw_axis){ 
    vec2 coord = frag_pos.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x,grid.y);
    float minimum_z = min(derivative.y, 1);
    float minimum_x = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    
    if(draw_axis){
        if(frag_pos.x > -0.1 * minimum_x && frag_pos.x < 0.1 * minimum_x){
            color.z = 1.0;
        }
        if(frag_pos.z > -0.1 * minimum_z && frag_pos.z < 0.1 * minimum_z){
            color.x = 1.0;
        };
    }
    return color;
}

float compute_depth(vec3 pos){
    vec4 clip_space_pos = v_proj_mat4 * v_view_mat4 * vec4(pos, 1.0);
    return clip_space_pos.z / clip_space_pos.w;
}

float compute_linear_depth(vec3 pos){
    vec4 clip_space_pos = v_proj_mat4 * v_view_mat4 * vec4(pos,1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0;
    float linear_depth = (2.0 * v_near_plane * v_far_plane) / (v_far_plane + v_near_plane - clip_space_depth * (v_far_plane - v_near_plane));
    return linear_depth / v_far_plane;
}