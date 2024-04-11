#include "pch.hpp"
#include "light_component.hpp"

Arcadia::LightComponent::LightComponent(const nlohmann::json& json)
{
    std::string type_str = json.at("type");
    auto& json_light = json.at("light");
    Light = Arcadia::Match<Arcadia::LightType>(
        type_str,
        "null"s,
        [&]()
    {
        return Arcadia::LightType{ Arcadia::NullLight{} };
    },
        "spot"s,
        [&]()
    {
        Arcadia::SpotLight light{};
        light.Position          = Arcadia::Vec3::FromJson(json_light.at("position"));
        light.Direction         = Arcadia::Vec3::FromJson(json_light.at("direction"));
        light.AttenuationCoefs = Arcadia::Vec3::FromJson(json_light.at("attenuation_coefs"));
        light.CutoffAngles     = Arcadia::Vec2::FromJson(json_light.at("cutoff_angles"));
        light.Color             = Arcadia::Vec3::FromJson(json_light.at("color"));
        light.AmbientStrength  = Arcadia::Vec3::FromJson(json_light.at("ambient_strength"));
        light.DiffuseStrength  = Arcadia::Vec3::FromJson(json_light.at("diffuse_strength"));
        light.SpecularStrength = Arcadia::Vec3::FromJson(json_light.at("specular_strength"));
        return Arcadia::LightType{ light };
    },
        "direct"s,
        [&]()
    {
        Arcadia::DirectLight light{};
        light.Direction         = Arcadia::Vec3::FromJson(json_light.at("direction"));
        light.Color             = Arcadia::Vec3::FromJson(json_light.at("color"));
        light.AmbientStrength  = Arcadia::Vec3::FromJson(json_light.at("ambient_strength"));
        light.DiffuseStrength  = Arcadia::Vec3::FromJson(json_light.at("diffuse_strength"));
        light.SpecularStrength = Arcadia::Vec3::FromJson(json_light.at("specular_strength"));
        return Arcadia::LightType{ light };
    },
        "area"s,
        [&]()
    {
        Arcadia::AreaLight light{};
        light.Position          = Arcadia::Vec3::FromJson(json_light.at("position"));
        light.Direction         = Arcadia::Vec3::FromJson(json_light.at("direction"));
        light.Size              = Arcadia::Vec2::FromJson(json_light.at("size"));
        light.Color             = Arcadia::Vec3::FromJson(json_light.at("color"));
        light.AmbientStrength  = Arcadia::Vec3::FromJson(json_light.at("ambient_strength"));
        light.DiffuseStrength  = Arcadia::Vec3::FromJson(json_light.at("diffuse_strength"));
        light.SpecularStrength = Arcadia::Vec3::FromJson(json_light.at("specular_strength"));
        return Arcadia::LightType{ light };
    },
        "point"s,
        [&]()
    {
        Arcadia::PointLight light{};
        light.Position          = Arcadia::Vec3::FromJson(json_light.at("position"));
        light.AttenuationCoefs = Arcadia::Vec3::FromJson(json_light.at("attenuation_coefs"));
        light.Color             = Arcadia::Vec3::FromJson(json_light.at("color"));
        light.AmbientStrength  = Arcadia::Vec3::FromJson(json_light.at("ambient_strength"));
        light.DiffuseStrength  = Arcadia::Vec3::FromJson(json_light.at("diffuse_strength"));
        light.SpecularStrength = Arcadia::Vec3::FromJson(json_light.at("specular_strength"));
        return Arcadia::LightType{ light };
    }
    );
}

auto Arcadia::LightComponent::ToJson() const -> nlohmann::json
{
    return Arcadia::Match<nlohmann::json>(
        Light,
        [&](const Arcadia::NullLight&)
    {
        return nlohmann::json{
            {"type","null"},
            {"light",nullptr}
        };
    },
        [&](const Arcadia::SpotLight& light)
    {
        return nlohmann::json{
            {"type","spot"},
            {"light",{
                    {"position"         ,Arcadia::Vec3::ToJson(light.Position)},
                    {"direction"        ,Arcadia::Vec3::ToJson(light.Direction)},
                    {"attenuation_coefs",Arcadia::Vec3::ToJson(light.AttenuationCoefs)},
                    {"cutoff_angles"    ,Arcadia::Vec2::ToJson(light.CutoffAngles)},
                    {"color"            ,Arcadia::Vec3::ToJson(light.Color)},
                    {"ambient_strength" ,Arcadia::Vec3::ToJson(light.AmbientStrength)},
                    {"diffuse_strength" ,Arcadia::Vec3::ToJson(light.DiffuseStrength)},
                    {"specular_strength",Arcadia::Vec3::ToJson(light.SpecularStrength)}
                }
            }
        };
    },
        [&](const Arcadia::AreaLight& light)
    {
        return nlohmann::json{
            {"type","area"},
            {"light",{
                    {"position"         ,Arcadia::Vec3::ToJson(light.Position)},
                    {"direction"        ,Arcadia::Vec3::ToJson(light.Direction)},
                    {"size"             ,Arcadia::Vec2::ToJson(light.Size)},
                    {"color"            ,Arcadia::Vec3::ToJson(light.Color)},
                    {"ambient_strength" ,Arcadia::Vec3::ToJson(light.AmbientStrength)},
                    {"diffuse_strength" ,Arcadia::Vec3::ToJson(light.DiffuseStrength)},
                    {"specular_strength",Arcadia::Vec3::ToJson(light.SpecularStrength)}
                }
            }
        };
    },
        [&](const Arcadia::DirectLight& light)
    {
        return nlohmann::json{
            {"type","direct"},
            {"light",{
                    {"direction"        ,Arcadia::Vec3::ToJson(light.Direction)},
                    {"color"            ,Arcadia::Vec3::ToJson(light.Color)},
                    {"ambient_strength" ,Arcadia::Vec3::ToJson(light.AmbientStrength)},
                    {"diffuse_strength" ,Arcadia::Vec3::ToJson(light.DiffuseStrength)},
                    {"specular_strength",Arcadia::Vec3::ToJson(light.SpecularStrength)}
                }
            }
        };
    },
        [&](const Arcadia::PointLight& light)
    {
        return nlohmann::json{
            {"type","point"},
            {"light",{
                    {"position"         ,Arcadia::Vec3::ToJson(light.Position)},
                    {"attenuation_coefs",Arcadia::Vec3::ToJson(light.AttenuationCoefs)},
                    {"color"            ,Arcadia::Vec3::ToJson(light.Color)},
                    {"ambient_strength" ,Arcadia::Vec3::ToJson(light.AmbientStrength)},
                    {"diffuse_strength" ,Arcadia::Vec3::ToJson(light.DiffuseStrength)},
                    {"specular_strength",Arcadia::Vec3::ToJson(light.SpecularStrength)}
                }
            }
        };
    }
    );
}

auto Arcadia::LightComponent::OnSnapshot() const -> memento_data_type
{
    memento_data_type memento{};

    memento.Light = Light;

    return memento;
}

void Arcadia::LightComponent::OnRestore(const memento_data_type& memento)
{
    Light = memento.Light;
}

