#include "pch.hpp"
#include "light_component.hpp"

#include"core/conditional.hpp"

arcadia::light_component::light_component(const nlohmann::json& json)
{
    std::string type_str = json.at("type");
    auto& json_light = json.at("light");
    light = arcadia::match<arcadia::light_type>(
        type_str,
        "spot"s,
        [&]() -> arcadia::light_type
    {
        arcadia::spot_light light{};
        light.position = arcadia::vec3::from_json(json_light.at("position"));
        light.direction = arcadia::vec3::from_json(json_light.at("direction"));
        light.cutoff_angle = json_light.at("cutoff_angle");
        light.color = arcadia::vec3::from_json(json_light.at("color"));
        light.ambient_strength = arcadia::vec3::from_json(json_light.at("ambient_strength"));
        light.diffuse_strength = arcadia::vec3::from_json(json_light.at("diffuse_strength"));
        light.specular_strength = arcadia::vec3::from_json(json_light.at("specular_strength"));
        return light;
    },
        "direct"s,
        [&]() -> arcadia::light_type
    {
        arcadia::direct_light light{};
        light.position = arcadia::vec3::from_json(json_light.at("position"));
        light.direction = arcadia::vec3::from_json(json_light.at("direction"));
        light.color = arcadia::vec3::from_json(json_light.at("color"));
        light.ambient_strength = arcadia::vec3::from_json(json_light.at("ambient_strength"));
        light.diffuse_strength = arcadia::vec3::from_json(json_light.at("diffuse_strength"));
        light.specular_strength = arcadia::vec3::from_json(json_light.at("specular_strength"));
        return light;
    },
        "area"s,
        [&]() -> arcadia::light_type
    {
        arcadia::area_light light{};
        light.position = arcadia::vec3::from_json(json_light.at("position"));
        light.direction = arcadia::vec3::from_json(json_light.at("direction"));
        light.size = arcadia::vec2::from_json(json_light.at("size"));
        light.color = arcadia::vec3::from_json(json_light.at("color"));
        light.ambient_strength = arcadia::vec3::from_json(json_light.at("ambient_strength"));
        light.diffuse_strength = arcadia::vec3::from_json(json_light.at("diffuse_strength"));
        light.specular_strength = arcadia::vec3::from_json(json_light.at("specular_strength"));
        return light;
    },
        "point"s,
        [&]() -> arcadia::light_type
    {
        arcadia::point_light light{};
        light.position = arcadia::vec3::from_json(json_light.at("position"));
        light.attenuation_coefs = arcadia::vec3::from_json(json_light.at("attenuation_coefs"));
        light.color = arcadia::vec3::from_json(json_light.at("color"));
        light.ambient_strength = arcadia::vec3::from_json(json_light.at("ambient_strength"));
        light.diffuse_strength = arcadia::vec3::from_json(json_light.at("diffuse_strength"));
        light.specular_strength = arcadia::vec3::from_json(json_light.at("specular_strength"));
        return light;
    }
    );
}

auto arcadia::light_component::to_json() const -> nlohmann::json
{
    return arcadia::match<nlohmann::json>(
        light,
        [&](const arcadia::spot_light& light)
    {
        return nlohmann::json{
            {"type","spot"},
            {"light",{
                    {"position",arcadia::vec3::to_json(light.position)},
                    {"direction",arcadia::vec3::to_json(light.direction)},
                    {"cutoff_angle",light.cutoff_angle},
                    {"color",arcadia::vec3::to_json(light.color)},
                    {"ambient_strength",arcadia::vec3::to_json(light.ambient_strength)},
                    {"diffuse_strength",arcadia::vec3::to_json(light.diffuse_strength)},
                    {"specular_strength",arcadia::vec3::to_json(light.specular_strength)}
                }
            }
        };
    },
        [&](const arcadia::area_light& light)
    {
        return nlohmann::json{
            {"type","area"},
            {"light",{
                    {"position",arcadia::vec3::to_json(light.position)},
                    {"direction",arcadia::vec3::to_json(light.direction)},
                    {"size",arcadia::vec2::to_json(light.size)},
                    {"color",arcadia::vec3::to_json(light.color)},
                    {"ambient_strength",arcadia::vec3::to_json(light.ambient_strength)},
                    {"diffuse_strength",arcadia::vec3::to_json(light.diffuse_strength)},
                    {"specular_strength",arcadia::vec3::to_json(light.specular_strength)}
                }
            }
        };
    },
        [&](const arcadia::direct_light& light)
    {
        return nlohmann::json{
            {"type","direct"},
            {"light",{
                    {"position",arcadia::vec3::to_json(light.position)},
                    {"direction",arcadia::vec3::to_json(light.direction)},
                    {"color",arcadia::vec3::to_json(light.color)},
                    {"ambient_strength",arcadia::vec3::to_json(light.ambient_strength)},
                    {"diffuse_strength",arcadia::vec3::to_json(light.diffuse_strength)},
                    {"specular_strength",arcadia::vec3::to_json(light.specular_strength)}
                }
            }
        };
    },
        [&](const arcadia::point_light& light)
    {
        return nlohmann::json{
            {"type","point"},
            {"light",{
                    {"position",arcadia::vec3::to_json(light.position)},
                    {"attenuation_coefs",arcadia::vec3::to_json(light.attenuation_coefs)},
                    {"color",arcadia::vec3::to_json(light.color)},
                    {"ambient_strength",arcadia::vec3::to_json(light.ambient_strength)},
                    {"diffuse_strength",arcadia::vec3::to_json(light.diffuse_strength)},
                    {"specular_strength",arcadia::vec3::to_json(light.specular_strength)}
                }
            }
        };
    }
    );
}
