#include "pch.hpp"

#include "light_component.hpp"

#include"core/match.hpp"

Arcadia::LightComponent::LightComponent(const nlohmann::json& json)
{
    std::string type_string = json.at("type");
    const nlohmann::json& json_light = json.at("light");
    Light = Match<LightType>(
        type_string,
        "null",
        [&]()
    {
        return LightType(NullLight{});
    },
        "spot",
        [&]()
    {
        SpotLight light{};
        light.AttenuationCoefficients = GlmVec3::FromJson(json_light.at("attenuation_coefs"));
        light.CutoffAngles     = GlmVec2::FromJson(json_light.at("cutoff_angles"));
        light.Color             = GlmVec3::FromJson(json_light.at("color"));
        light.AmbientStrength  = GlmVec3::FromJson(json_light.at("ambient_strength"));
        light.DiffuseStrength  = GlmVec3::FromJson(json_light.at("diffuse_strength"));
        light.SepcularStrength = GlmVec3::FromJson(json_light.at("specular_strength"));
        return LightType(light);
    },
        "direct",
        [&]()
    {
        DirectLight light{};
        light.Color            = GlmVec3::FromJson(json_light.at("color"));
        light.AmbientStrength  = GlmVec3::FromJson(json_light.at("ambient_strength"));
        light.DiffuseStrength  = GlmVec3::FromJson(json_light.at("diffuse_strength"));
        light.SepcularStrength = GlmVec3::FromJson(json_light.at("specular_strength"));
        return LightType(light);
    },
        "area",
        [&]()
    {
        AreaLight light{};
        light.Size             = GlmVec2::FromJson(json_light.at("size"));
        light.Color            = GlmVec3::FromJson(json_light.at("color"));
        light.AmbientStrength  = GlmVec3::FromJson(json_light.at("ambient_strength"));
        light.DiffuseStrength  = GlmVec3::FromJson(json_light.at("diffuse_strength"));
        light.SepcularStrength = GlmVec3::FromJson(json_light.at("specular_strength"));
        return LightType(light);
    },
        "point",
        [&]()
    {
        PointLight light{};
        light.AttenuationCoefficients = GlmVec3::FromJson(json_light.at("attenuation_coefs"));
        light.Color            = GlmVec3::FromJson(json_light.at("color"));
        light.AmbientStrength  = GlmVec3::FromJson(json_light.at("ambient_strength"));
        light.DiffuseStrength  = GlmVec3::FromJson(json_light.at("diffuse_strength"));
        light.SepcularStrength = GlmVec3::FromJson(json_light.at("specular_strength"));
        return LightType(light);
    }
    );
}

auto Arcadia::LightComponent::ToJson() const -> nlohmann::json
{
    return MatchVariant<nlohmann::json>(
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
                    {"attenuation_coefs",GlmVec3::ToJson(light.AttenuationCoefficients)},
                    {"cutoff_angles"    ,GlmVec2::ToJson(light.CutoffAngles)},
                    {"color"            ,GlmVec3::ToJson(light.Color)},
                    {"ambient_strength" ,GlmVec3::ToJson(light.AmbientStrength)},
                    {"diffuse_strength" ,GlmVec3::ToJson(light.DiffuseStrength)},
                    {"specular_strength",GlmVec3::ToJson(light.SepcularStrength)}
                }
            }
        };
    },
        [&](const AreaLight& light)
    {
        return nlohmann::json{
            {"type","area"},
            {"light",{
                    {"size"             ,GlmVec2::ToJson(light.Size)},
                    {"color"            ,GlmVec3::ToJson(light.Color)},
                    {"ambient_strength" ,GlmVec3::ToJson(light.AmbientStrength)},
                    {"diffuse_strength" ,GlmVec3::ToJson(light.DiffuseStrength)},
                    {"specular_strength",GlmVec3::ToJson(light.SepcularStrength)}
                }
            }
        };
    },
        [&](const DirectLight& light)
    {
        return nlohmann::json{
            {"type","direct"},
            {"light",{
                    {"color"            ,GlmVec3::ToJson(light.Color)},
                    {"ambient_strength" ,GlmVec3::ToJson(light.AmbientStrength)},
                    {"diffuse_strength" ,GlmVec3::ToJson(light.DiffuseStrength)},
                    {"specular_strength",GlmVec3::ToJson(light.SepcularStrength)}
                }
            }
        };
    },
        [&](const PointLight& light)
    {
        return nlohmann::json{
            {"type","point"},
            {"light",{
                    {"attenuation_coefs",GlmVec3::ToJson(light.AttenuationCoefficients)},
                    {"color"            ,GlmVec3::ToJson(light.Color)},
                    {"ambient_strength" ,GlmVec3::ToJson(light.AmbientStrength)},
                    {"diffuse_strength" ,GlmVec3::ToJson(light.DiffuseStrength)},
                    {"specular_strength",GlmVec3::ToJson(light.SepcularStrength)}
                }
            }
        };
    }
    );
}

auto Arcadia::LightComponent::OnSnapshot() const -> std::shared_ptr<MementoDataBase>
{
    std::shared_ptr< LightComponentMementoData> memento_data_sptr = std::make_shared<LightComponentMementoData>();

    memento_data_sptr->Light = Light;

    return memento_data_sptr;
}

void Arcadia::LightComponent::OnRestore(const std::shared_ptr<MementoDataBase>& memento_data_sptr)
{
    LightComponentMementoData& memento_data = memento_data_sptr->CastTo<LightComponentMementoData>();

    Light = memento_data.Light;
}