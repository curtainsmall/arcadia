#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"resource/components/component_interface.hpp"

struct NullLight
{
public:
    auto operator==(const NullLight&) const -> bool = default;
};

struct SpotLight
{
public:
    auto operator==(const SpotLight&) const -> bool = default;
public:
    glm::vec3 attenuation_coefs{ 1.f,.045f,.0075f };
    glm::vec2 cutoff_angles{ glm::radians(30.f),glm::radians(32.f) };
    glm::vec3 color{ 1.f,1.f,1.f };
    glm::vec3 ambient_strength{ vec3::zero() };
    glm::vec3 diffuse_strength{ 5.f,5.f,5.f };
    glm::vec3 specular_strength{ vec3::zero() };
};

struct DirectLight
{
public:
    auto operator==(const DirectLight&) const -> bool = default;
public:
    glm::vec3 color{ 1.f,1.f,1.f };
    glm::vec3 ambient_strength{ vec3::zero() };
    glm::vec3 diffuse_strength{ 5.f,5.f,5.f };
    glm::vec3 specular_strength{ vec3::zero() };
};

struct AreaLight
{
public:
    auto operator==(const AreaLight&) const -> bool = default;
public:
    glm::vec2 size{ vec2::zero() };
    glm::vec3 color{ 1.f,1.f,1.f };
    glm::vec3 ambient_strength{ vec3::zero() };
    glm::vec3 diffuse_strength{ 5.f,5.f,5.f };
    glm::vec3 specular_strength{ vec3::zero() };
};

struct PointLight
{
public:
    auto operator==(const PointLight&) const -> bool = default;
public:
    glm::vec3 attenuation_coefs{ 1.f,.045f,.0075f };
    glm::vec3 color{ 1.f,1.f,1.f };
    glm::vec3 ambient_strength{ vec3::zero() };
    glm::vec3 diffuse_strength{ 5.f,5.f,5.f };
    glm::vec3 specular_strength{ vec3::zero() };
};

using LightType = std::variant<
    NullLight,
    SpotLight,
    DirectLight,
    AreaLight,
    PointLight
>;

struct LightComponentMementoData: MementoDataBase
{
public:
    auto operator==(const LightComponentMementoData&) const -> bool = default;
public:
    LightType light{};
};

struct LightComponent:
    iComponent,
    iMementoOriginator
{
public:
    using self_type = LightComponent;
public:
    ACDA_COMPONENT_TYPE_STR_GETTERS("light");

    LightComponent() = default;
    LightComponent(const nlohmann::json& json);
    ~LightComponent() = default;
    [[nodiscard]]
    auto to_json() const->nlohmann::json;

protected:
    [[nodiscard]]
    virtual auto on_snapshot() const->std::shared_ptr<MementoDataBase> override;
    virtual void on_restore(const std::shared_ptr<MementoDataBase>& sp_memento_data) override;

public:
    LightType light{ NullLight{} };
};
