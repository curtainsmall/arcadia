#include "pch.hpp"

#include "light_component.hpp"

#include "core/match.hpp"

Arcadia::LightComponent::LightComponent(const nlohmann::json& json)
{
    std::string type_string = json.at("type");
    const nlohmann::json& json_light = json.at("light");
    _Light = Match<LightType>(
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
        light.SetAttenuationCoefficients(Glm::Vec3_FromJson(json_light.at("attenuation_coefs")));
        light.SetCutoffAngles(Glm::Vec2_FromJson(json_light.at("cutoff_angles")));
        light.SetColor(Glm::Vec3_FromJson(json_light.at("color")));
        light.SetAmbientStrength(Glm::Vec3_FromJson(json_light.at("ambient_strength")));
        light.SetDiffuseStrength(Glm::Vec3_FromJson(json_light.at("diffuse_strength")));
        light.SetSpecularStrength(Glm::Vec3_FromJson(json_light.at("specular_strength")));
        return LightType(light);
    },
        "direct",
        [&]()
    {
        DirectLight light{};
        light.SetColor(Glm::Vec3_FromJson(json_light.at("color")));
        light.SetAmbientStrength(Glm::Vec3_FromJson(json_light.at("ambient_strength")));
        light.SetDiffuseStrength(Glm::Vec3_FromJson(json_light.at("diffuse_strength")));
        light.SetSpecularStrength(Glm::Vec3_FromJson(json_light.at("specular_strength")));
        return LightType(light);
    },
        "area",
        [&]()
    {
        AreaLight light{};
        light.SetSize(Glm::Vec2_FromJson(json_light.at("size")));
        light.SetColor(Glm::Vec3_FromJson(json_light.at("color")));
        light.SetAmbientStrength(Glm::Vec3_FromJson(json_light.at("ambient_strength")));
        light.SetDiffuseStrength(Glm::Vec3_FromJson(json_light.at("diffuse_strength")));
        light.SetSpecularStrength(Glm::Vec3_FromJson(json_light.at("specular_strength")));
        return LightType(light);
    },
        "point",
        [&]()
    {
        PointLight light{};
        light.SetAttenuationCoefficients(Glm::Vec3_FromJson(json_light.at("attenuation_coefs")));
        light.SetColor(Glm::Vec3_FromJson(json_light.at("color")));
        light.SetAmbientStrength(Glm::Vec3_FromJson(json_light.at("ambient_strength")));
        light.SetDiffuseStrength(Glm::Vec3_FromJson(json_light.at("diffuse_strength")));
        light.SetSpecularStrength(Glm::Vec3_FromJson(json_light.at("specular_strength")));
        return LightType(light);
    }
    );
}

auto Arcadia::LightComponent::ToJson() const -> nlohmann::json
{
    return MatchVariant<nlohmann::json>(
        _Light,
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
                    {"attenuation_coefs",Glm::Vec3_ToJson(light.GetAttenuationCoefficients())},
                    {"cutoff_angles"    ,Glm::Vec2_ToJson(light.GetCutoffAngles())},
                    {"color"            ,Glm::Vec3_ToJson(light.GetColor())},
                    {"ambient_strength" ,Glm::Vec3_ToJson(light.GetAmbientStrength())},
                    {"diffuse_strength" ,Glm::Vec3_ToJson(light.GetDiffuseStrength())},
                    {"specular_strength",Glm::Vec3_ToJson(light.GetSpecularStrength())}
                }
            }
        };
    },
        [&](const AreaLight& light)
    {
        return nlohmann::json{
            {"type","area"},
            {"light",{
                    {"size"             ,Glm::Vec2_ToJson(light.GetSize())},
                    {"color"            ,Glm::Vec3_ToJson(light.GetColor())},
                    {"ambient_strength" ,Glm::Vec3_ToJson(light.GetAmbientStrength())},
                    {"diffuse_strength" ,Glm::Vec3_ToJson(light.GetDiffuseStrength())},
                    {"specular_strength",Glm::Vec3_ToJson(light.GetSpecularStrength())}
                }
            }
        };
    },
        [&](const DirectLight& light)
    {
        return nlohmann::json{
            {"type","direct"},
            {"light",{
                    {"color"            ,Glm::Vec3_ToJson(light.GetColor())},
                    {"ambient_strength" ,Glm::Vec3_ToJson(light.GetAmbientStrength())},
                    {"diffuse_strength" ,Glm::Vec3_ToJson(light.GetDiffuseStrength())},
                    {"specular_strength",Glm::Vec3_ToJson(light.GetSpecularStrength())}
                }
            }
        };
    },
        [&](const PointLight& light)
    {
        return nlohmann::json{
            {"type","point"},
            {"light",{
                    {"attenuation_coefs",Glm::Vec3_ToJson(light.GetAttenuationCoefficients())},
                    {"color"            ,Glm::Vec3_ToJson(light.GetColor())},
                    {"ambient_strength" ,Glm::Vec3_ToJson(light.GetAmbientStrength())},
                    {"diffuse_strength" ,Glm::Vec3_ToJson(light.GetDiffuseStrength())},
                    {"specular_strength",Glm::Vec3_ToJson(light.GetSpecularStrength())}
                }
            }
        };
    }
    );
}

auto Arcadia::LightComponent::GetLight() const -> const LightType&
{
    return _Light;
}

auto Arcadia::LightComponent::GetLight() -> LightType&
{
    return _Light;
}

void Arcadia::LightComponent::SetLight(const LightType& light)
{
    _Light = light;
}

auto Arcadia::SpotLight::GetAttenuationCoefficients() const -> const glm::vec3&
{
    return _AttenuationCoefficients;
}

void Arcadia::SpotLight::SetAttenuationCoefficients(const glm::vec3& coeffs)
{
    _AttenuationCoefficients = coeffs;
}

auto Arcadia::SpotLight::GetCutoffAngles() const -> const glm::vec2&
{
    return _CutoffAngles;
}

void Arcadia::SpotLight::SetCutoffAngles(const glm::vec2& angles)
{
    _CutoffAngles = angles;
}

auto Arcadia::SpotLight::GetColor() const -> const glm::vec3&
{
    return _Color;
}

void Arcadia::SpotLight::SetColor(const glm::vec3& color)
{
    _Color = color;
}

auto Arcadia::SpotLight::GetAmbientStrength() const -> const glm::vec3&
{
    return _AmbientStrength;
}

void Arcadia::SpotLight::SetAmbientStrength(const glm::vec3& strength)
{
    _AmbientStrength = strength;
}

auto Arcadia::SpotLight::GetDiffuseStrength() const -> const glm::vec3
{
    return _DiffuseStrength;
}

void Arcadia::SpotLight::SetDiffuseStrength(const glm::vec3& strength)
{
    _DiffuseStrength = strength;
}

auto Arcadia::SpotLight::GetSpecularStrength() const -> const glm::vec3&
{
    return _SpecularStrength;
}

void Arcadia::SpotLight::SetSpecularStrength(const glm::vec3& strength)
{
    _SpecularStrength = strength;
}

auto Arcadia::DirectLight::GetColor() const -> const glm::vec3&
{
    return _Color;
}

void Arcadia::DirectLight::SetColor(const glm::vec3& color)
{
    _Color = color;
}

auto Arcadia::DirectLight::GetAmbientStrength() const -> const glm::vec3&
{
    return _AmbientStrength;
}

void Arcadia::DirectLight::SetAmbientStrength(const glm::vec3& strength)
{
    _AmbientStrength = strength;
}

auto Arcadia::DirectLight::GetDiffuseStrength() const -> const glm::vec3
{
    return _DiffuseStrength;
}

void Arcadia::DirectLight::SetDiffuseStrength(const glm::vec3& strength)
{
    _DiffuseStrength = strength;
}

auto Arcadia::DirectLight::GetSpecularStrength() const -> const glm::vec3&
{
    return _SpecularStrength;
}

void Arcadia::DirectLight::SetSpecularStrength(const glm::vec3& strength)
{
    _SpecularStrength = strength;
}

auto Arcadia::AreaLight::GetSize() const -> const glm::vec2&
{
    return _Size;
}

void Arcadia::AreaLight::SetSize(const glm::vec2& size)
{
    _Size = size;
}

auto Arcadia::AreaLight::GetColor() const -> const glm::vec3&
{
    return _Color;
}

void Arcadia::AreaLight::SetColor(const glm::vec3& color)
{
    _Color = color;
}

auto Arcadia::AreaLight::GetAmbientStrength() const -> const glm::vec3&
{
    return _AmbientStrength;
}

void Arcadia::AreaLight::SetAmbientStrength(const glm::vec3& strength)
{
    _AmbientStrength = strength;
}

auto Arcadia::AreaLight::GetDiffuseStrength() const -> const glm::vec3
{
    return _DiffuseStrength;
}

void Arcadia::AreaLight::SetDiffuseStrength(const glm::vec3& strength)
{
    _DiffuseStrength = strength;
}

auto Arcadia::AreaLight::GetSpecularStrength() const -> const glm::vec3&
{
    return _SpecularStrength;
}

void Arcadia::AreaLight::SetSpecularStrength(const glm::vec3& strength)
{
    _SpecularStrength = strength;
}

auto Arcadia::PointLight::GetAttenuationCoefficients() const -> const glm::vec3&
{
    return _AttenuationCoefficients;
}

void Arcadia::PointLight::SetAttenuationCoefficients(const glm::vec3& coeffs)
{
    _AttenuationCoefficients = coeffs;
}

auto Arcadia::PointLight::GetColor() const -> const glm::vec3&
{
    return _Color;
}

void Arcadia::PointLight::SetColor(const glm::vec3& color)
{
    _Color = color;
}

auto Arcadia::PointLight::GetAmbientStrength() const -> const glm::vec3&
{
    return _AmbientStrength;
}

void Arcadia::PointLight::SetAmbientStrength(const glm::vec3& strength)
{
    _AmbientStrength = strength;
}

auto Arcadia::PointLight::GetDiffuseStrength() const -> const glm::vec3
{
    return _DiffuseStrength;
}

void Arcadia::PointLight::SetDiffuseStrength(const glm::vec3& strength)
{
    _DiffuseStrength = strength;
}

auto Arcadia::PointLight::GetSpecularStrength() const -> const glm::vec3&
{
    return _SpecularStrength;
}

void Arcadia::PointLight::SetSpecularStrength(const glm::vec3& strength)
{
    _SpecularStrength = strength;
}