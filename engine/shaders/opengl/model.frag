#version 430 core

in vec3 v_normal;
in vec2 v_tex_coord;
in vec3 v_frag_pos;

out vec4 o_color;

uniform vec3 u_view_pos;

// Textures
uniform struct {
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
} u_material;

// Lights
const int light_type_spot = 1;
const int light_type_direct = 2;
const int light_type_area = 3;
const int light_type_point = 4;
struct light_t 
{
    int type; // 0
    float cosine_inner_cutoff_angle; // 4
    float cosine_outer_cutoff_angle; // 8
    vec3 position; // 16
    vec3 direction;// 32
    vec3 attenuation_coefs;// 48
    vec3 color;// 64
    vec3 ambient_strength;// 80
    vec3 diffuse_strength;// 96
    vec3 specular_strength;// 112
    // size: 128
};
const int max_light_count = 32;
layout(std140, binding = 0) uniform lights_t
{
    int u_light_count; //0
    light_t u_lights[max_light_count]; // 16
};

vec3 calc_spot_light(light_t light,vec3 normal,vec3 view_dir);
vec3 calc_direct_light(light_t light,vec3 normal, vec3 view_dir);
vec3 calc_point_light(light_t light, vec3 normal, vec3 view_dir);

void main(){
    
    vec3 res_color = vec3(0,0,0);

    // Point light
    for(int i = 0; i < u_light_count; ++i)
    {
        // The following conditional branchings are all dependent on only uniform values and constant values which should not take much overhead
        if(u_lights[i].type == light_type_spot)
        {
            res_color += calc_spot_light(u_lights[i], v_normal, normalize(u_view_pos - v_frag_pos));
        }
        else if(u_lights[i].type == light_type_direct)
        {
            res_color += calc_direct_light(u_lights[i], v_normal, normalize(u_view_pos - v_frag_pos));
        }
        else if(u_lights[i].type == light_type_area)
        {
        }
        else if(u_lights[i].type == light_type_point) 
        {
            res_color += calc_point_light(u_lights[i], v_normal, normalize(u_view_pos - v_frag_pos));
        }
    }

    o_color = vec4(res_color,1.f); 
}


vec3 calc_spot_light(light_t light,vec3 normal,vec3 view_dir)
{
    vec3 light_dir = normalize(light.position - v_frag_pos);

    // Spot light intensity
    float cosine_theta = dot(light_dir, normalize(-light.direction));
    float cosine_epsilon = light.cosine_inner_cutoff_angle - light.cosine_outer_cutoff_angle;
    float intensity = clamp((cosine_theta - light.cosine_outer_cutoff_angle) / cosine_epsilon, 0.0, 1.0);

    // Shading for ambient
    float diff = max(dot(normal, light_dir), 0);

    // Shader for specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir,reflect_dir),0), u_material.shininess);

    // Attenuation
    float dist = length(light.position - v_frag_pos);
    float attenuation = 1.0 / (light.attenuation_coefs.x + light.attenuation_coefs.y * dist + light.attenuation_coefs.z * (dist * dist));

    // Combine 
    vec3 ambient = light.ambient_strength * vec3(texture(u_material.ambient, v_tex_coord));
    vec3 diffuse = light.diffuse_strength * diff * vec3(texture(u_material.diffuse,v_tex_coord));
    vec3 specular = light.specular_strength * spec * vec3(texture(u_material.specular,v_tex_coord));
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular) * light.color * intensity;
}

vec3 calc_direct_light(light_t light,vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(-light.direction);
    
    // Shading for ambient
    float diff = max(dot(normal,light_dir),0);

    // Shading for specular
    vec3 reflect_dir = reflect(-light_dir,normal);
    float spec = pow(max(dot(view_dir,reflect_dir),0),u_material.shininess);

    // Combine
    vec3 ambient = light.ambient_strength * vec3(texture(u_material.ambient, v_tex_coord));
    vec3 diffuse = light.diffuse_strength * diff * vec3(texture(u_material.diffuse,v_tex_coord));
    vec3 specular = light.specular_strength * spec * vec3(texture(u_material.specular,v_tex_coord));

    return (ambient + diffuse + specular) * light.color;
}

vec3 calc_point_light(light_t light, vec3 normal, vec3 view_dir)
{

    vec3 light_dir = normalize(light.position - v_frag_pos);

    // Shading for ambient
    float diff = max(dot(normal, light_dir), 0);

    // Shader for specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir,reflect_dir),0), u_material.shininess);

    // Attenuation
    float dist = length(light.position - v_frag_pos);
    float attenuation = 1.0 / (light.attenuation_coefs.x + light.attenuation_coefs.y * dist + light.attenuation_coefs.z * (dist * dist));

    // Combine 
    vec3 ambient = light.ambient_strength * vec3(texture(u_material.ambient, v_tex_coord));
    vec3 diffuse = light.diffuse_strength * diff * vec3(texture(u_material.diffuse,v_tex_coord));
    vec3 specular = light.specular_strength * spec * vec3(texture(u_material.specular,v_tex_coord));
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular) * light.color;
}
