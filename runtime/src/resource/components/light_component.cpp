#include "pch.hpp"

#include "light_component.hpp"

LightComponent::LightComponent(const nlohmann::json& json)
{
    std::string type_str = json.at("type");
    auto& json_light = json.at("light");
    light = match<LightType>(
        type_str,
        "null"s,
        [&]()
    {
        return LightType{ NullLight{} };
    },
        "spot"s,
        [&]()
    {
        SpotLight light{};
        light.attenuation_coefs = vec3::from_json(json_light.at("attenuation_coefs"));
        light.cutoff_angles     = vec2::from_json(json_light.at("cutoff_angles"));
        light.color             = vec3::from_json(json_light.at("color"));
        light.ambient_strength  = vec3::from_json(json_light.at("ambient_strength"));
        light.diffuse_strength  = vec3::from_json(json_light.at("diffuse_strength"));
        light.specular_strength = vec3::from_json(json_light.at("specular_strength"));
        return LightType{ light };
    },
        "direct"s,
        [&]()
    {
        DirectLight light{};
        light.color            = vec3::from_json(json_light.at("color"));
        light.ambient_strength  = vec3::from_json(json_light.at("ambient_strength"));
        light.diffuse_strength  = vec3::from_json(json_light.at("diffuse_strength"));
        light.specular_strength = vec3::from_json(json_light.at("specular_strength"));
        return LightType{ light };
    },
        "area"s,
        [&]()
    {
        AreaLight light{};
        light.size             = vec2::from_json(json_light.at("size"));
        light.color            = vec3::from_json(json_light.at("color"));
        light.ambient_strength  = vec3::from_json(json_light.at("ambient_strength"));
        light.diffuse_strength  = vec3::from_json(json_light.at("diffuse_strength"));
        light.specular_strength = vec3::from_json(json_light.at("specular_strength"));
        return LightType{ light };
    },
        "point"s,
        [&]()
    {
        PointLight light{};
        light.attenuation_coefs = vec3::from_json(json_light.at("attenuation_coefs"));
        light.color            = vec3::from_json(json_light.at("color"));
        light.ambient_strength  = vec3::from_json(json_light.at("ambient_strength"));
        light.diffuse_strength  = vec3::from_json(json_light.at("diffuse_strength"));
        light.specular_strength = vec3::from_json(json_light.at("specular_strength"));
        return LightType{ light };
    }
    );
}

auto LightComponent::to_json() const -> nlohmann::json
{
    return match<nlohmann::json>(
        light,
        [&](const NullLight&)
    {
        return nlohmann::json{
            {"type","null"},
            {"light",nullptr}
        };
    },
        [&](const SpotLight& light)
    {
        return nlohmann::json{
            {"type","spot"},
            {"light",{
                    {"attenuation_coefs",vec3::to_json(light.attenuation_coefs)},
                    {"cutoff_angles"    ,vec2::to_json(light.cutoff_angles)},
                    {"color"            ,vec3::to_json(light.color)},
                    {"ambient_strength" ,vec3::to_json(light.ambient_strength)},
                    {"diffuse_strength" ,vec3::to_json(light.diffuse_strength)},
                    {"specular_strength",vec3::to_json(light.specular_strength)}
                }
            }
        };
    },
        [&](const AreaLight& light)
    {
        return nlohmann::json{
            {"type","area"},
            {"light",{
                    {"size"             ,vec2::to_json(light.size)},
                    {"color"            ,vec3::to_json(light.color)},
                    {"ambient_strength" ,vec3::to_json(light.ambient_strength)},
                    {"diffuse_strength" ,vec3::to_json(light.diffuse_strength)},
                    {"specular_strength",vec3::to_json(light.specular_strength)}
                }
            }
        };
    },
        [&](const DirectLight& light)
    {
        return nlohmann::json{
            {"type","direct"},
            {"light",{
                    {"color"            ,vec3::to_json(light.color)},
                    {"ambient_strength" ,vec3::to_json(light.ambient_strength)},
                    {"diffuse_strength" ,vec3::to_json(light.diffuse_strength)},
                    {"specular_strength",vec3::to_json(light.specular_strength)}
                }
            }
        };
    },
        [&](const PointLight& light)
    {
        return nlohmann::json{
            {"type","point"},
            {"light",{
                    {"attenuation_coefs",vec3::to_json(light.attenuation_coefs)},
                    {"color"            ,vec3::to_json(light.color)},
                    {"ambient_strength" ,vec3::to_json(light.ambient_strength)},
                    {"diffuse_strength" ,vec3::to_json(light.diffuse_strength)},
                    {"specular_strength",vec3::to_json(light.specular_strength)}
                }
            }
        };
    }
    );
}

auto LightComponent::on_snapshot() const -> std::shared_ptr<MementoDataBase>
{
    auto sp_memento_data = std::make_shared<LightComponentMementoData>();

    sp_memento_data->light = light;

    return sp_memento_data;
}

void LightComponent::on_restore(const std::shared_ptr<MementoDataBase>& sp_memento_data)
{
    auto& memento_data = sp_memento_data->as<LightComponentMementoData>();

    light = memento_data.light;
}

