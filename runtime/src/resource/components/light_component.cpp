#include "pch.hpp"

#include "light_component.hpp"

Arcadia::LightComponent::LightComponent(const nlohmann::json& json)
{
    std::string type_str = json.at("type");
    auto& json_light = json.at("light");
    Light = Match<LightType>(
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
        light.AttenuationCoefficients = Vec3::FromJson(json_light.at("attenuation_coefs"));
        light.CutoffAngles     = Vec2::FromJson(json_light.at("cutoff_angles"));
        light.Color             = Vec3::FromJson(json_light.at("color"));
        light.AmbientStrength  = Vec3::FromJson(json_light.at("ambient_strength"));
        light.DiffuseStrength  = Vec3::FromJson(json_light.at("diffuse_strength"));
        light.SepcularStrength = Vec3::FromJson(json_light.at("specular_strength"));
        return LightType{ light };
    },
        "direct"s,
        [&]()
    {
        DirectLight light{};
        light.Color            = Vec3::FromJson(json_light.at("color"));
        light.AmbientStrength  = Vec3::FromJson(json_light.at("ambient_strength"));
        light.DiffuseStrength  = Vec3::FromJson(json_light.at("diffuse_strength"));
        light.SepcularStrength = Vec3::FromJson(json_light.at("specular_strength"));
        return LightType{ light };
    },
        "area"s,
        [&]()
    {
        AreaLight light{};
        light.Size             = Vec2::FromJson(json_light.at("size"));
        light.Color            = Vec3::FromJson(json_light.at("color"));
        light.AmbientStrength  = Vec3::FromJson(json_light.at("ambient_strength"));
        light.DiffuseStrength  = Vec3::FromJson(json_light.at("diffuse_strength"));
        light.SepcularStrength = Vec3::FromJson(json_light.at("specular_strength"));
        return LightType{ light };
    },
        "point"s,
        [&]()
    {
        PointLight light{};
        light.AttenuationCoefficients = Vec3::FromJson(json_light.at("attenuation_coefs"));
        light.Color            = Vec3::FromJson(json_light.at("color"));
        light.AmbientStrength  = Vec3::FromJson(json_light.at("ambient_strength"));
        light.DiffuseStrength  = Vec3::FromJson(json_light.at("diffuse_strength"));
        light.SepcularStrength = Vec3::FromJson(json_light.at("specular_strength"));
        return LightType{ light };
    }
    );
}

auto Arcadia::LightComponent::ToJson() const -> nlohmann::json
{
    return Match<nlohmann::json>(
        Light,
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
                    {"attenuation_coefs",Vec3::ToJson(light.AttenuationCoefficients)},
                    {"cutoff_angles"    ,Vec2::ToJson(light.CutoffAngles)},
                    {"color"            ,Vec3::ToJson(light.Color)},
                    {"ambient_strength" ,Vec3::ToJson(light.AmbientStrength)},
                    {"diffuse_strength" ,Vec3::ToJson(light.DiffuseStrength)},
                    {"specular_strength",Vec3::ToJson(light.SepcularStrength)}
                }
            }
        };
    },
        [&](const AreaLight& light)
    {
        return nlohmann::json{
            {"type","area"},
            {"light",{
                    {"size"             ,Vec2::ToJson(light.Size)},
                    {"color"            ,Vec3::ToJson(light.Color)},
                    {"ambient_strength" ,Vec3::ToJson(light.AmbientStrength)},
                    {"diffuse_strength" ,Vec3::ToJson(light.DiffuseStrength)},
                    {"specular_strength",Vec3::ToJson(light.SepcularStrength)}
                }
            }
        };
    },
        [&](const DirectLight& light)
    {
        return nlohmann::json{
            {"type","direct"},
            {"light",{
                    {"color"            ,Vec3::ToJson(light.Color)},
                    {"ambient_strength" ,Vec3::ToJson(light.AmbientStrength)},
                    {"diffuse_strength" ,Vec3::ToJson(light.DiffuseStrength)},
                    {"specular_strength",Vec3::ToJson(light.SepcularStrength)}
                }
            }
        };
    },
        [&](const PointLight& light)
    {
        return nlohmann::json{
            {"type","point"},
            {"light",{
                    {"attenuation_coefs",Vec3::ToJson(light.AttenuationCoefficients)},
                    {"color"            ,Vec3::ToJson(light.Color)},
                    {"ambient_strength" ,Vec3::ToJson(light.AmbientStrength)},
                    {"diffuse_strength" ,Vec3::ToJson(light.DiffuseStrength)},
                    {"specular_strength",Vec3::ToJson(light.SepcularStrength)}
                }
            }
        };
    }
    );
}

auto Arcadia::LightComponent::OnSnapshot() const -> std::shared_ptr<MementoDataBase>
{
    auto sp_memento_data = std::make_shared<LightComponentMementoData>();

    sp_memento_data->Light = Light;

    return sp_memento_data;
}

void Arcadia::LightComponent::OnRestore(const std::shared_ptr<MementoDataBase>& sp_memento_data)
{
    auto& memento_data = sp_memento_data->CastTo<LightComponentMementoData>();

    Light = memento_data.Light;
}