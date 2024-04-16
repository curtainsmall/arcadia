#version 330 core

in vec3 v_normal;
in vec2 v_tex_coord;

out vec4 out_color;

uniform sampler2D u_tex_ambient;
uniform sampler2D u_tex_diffuse;
uniform sampler2D u_tex_specular;

void main(){
        vec3 ambient = vec3(texture(u_tex_ambient,v_tex_coord));
        vec3 diffuse = vec3(texture(u_tex_diffuse,v_tex_coord));
        vec3 specular = vec3(texture(u_tex_specular,v_tex_coord));
        out_color = vec4(ambient + diffuse + specular,1.f); 
}






