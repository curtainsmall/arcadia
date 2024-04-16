#version 330 core

in vec3 v_tex_coord;

out vec4 out_color;

uniform samplerCube u_skybox;

void main(){
    out_color = texture(u_skybox,v_tex_coord);
}

