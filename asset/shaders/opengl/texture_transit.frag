#version 330 core

in vec2 v_tex_coord;

out vec4 out_color;

uniform sampler2D u_tex_diffuse;

void main(){
   // out_color = vec4(.5f,.5f,.5f,1.f);
    out_color = texture(u_tex_diffuse,v_tex_coord);
}