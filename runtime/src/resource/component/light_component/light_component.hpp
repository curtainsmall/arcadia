#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"resource/component/component.hpp"

namespace Arcadia
{
    struct ARCADIA_API NullLight
    {
    public:
        auto operator==(const NullLight&) const -> bool = default;
    };

    struct ARCADIA_API SpotLight
    {
    public:
        auto operator==(const SpotLight&) const -> bool = default;
    public:
        glm::vec3 Position{ Arcadia::Vec3::Zero() };
        glm::vec3 Direction{ Arcadia::Vec3::Zero() };
        glm::vec3 AttenuationCoefs{ 1.f,.045f,.0075f };
        glm::vec2 CutoffAngles{ glm::radians(30.f),glm::radians(32.f) };
        glm::vec3 Color{ 1.f,1.f,1.f };
        glm::vec3 AmbientStrength{ Arcadia::Vec3::Zero() };
        glm::vec3 DiffuseStrength{ 5.f,5.f,5.f };
        glm::vec3 SpecularStrength{ Arcadia::Vec3::Zero() };
    };

    struct ARCADIA_API DirectLight
    {
    public:
        auto operator==(const DirectLight&) const -> bool = default;
    public:
        glm::vec3 Direction{ Arcadia::Vec3::Zero() };
        glm::vec3 Color{ 1.f,1.f,1.f };
        glm::vec3 AmbientStrength{ Arcadia::Vec3::Zero() };
        glm::vec3 DiffuseStrength{ 5.f,5.f,5.f };
        glm::vec3 SpecularStrength{ Arcadia::Vec3::Zero() };
    };

    struct ARCADIA_API AreaLight
    {
    public:
        auto operator==(const AreaLight&) const -> bool = default;
    public:
        glm::vec3 Position{ Arcadia::Vec3::Zero() };
        glm::vec3 Direction{ Arcadia::Vec3::Zero() };
        glm::vec2 Size{ Arcadia::Vec2::Zero() };
        glm::vec3 Color{ 1.f,1.f,1.f };
        glm::vec3 AmbientStrength{ Arcadia::Vec3::Zero() };
        glm::vec3 DiffuseStrength{ 5.f,5.f,5.f };
        glm::vec3 SpecularStrength{ Arcadia::Vec3::Zero() };
    };

    struct ARCADIA_API PointLight
    {
    public:
        auto operator==(const PointLight&) const -> bool = default;
    public:
        glm::vec3 Position{ Arcadia::Vec3::Zero() };
        glm::vec3 AttenuationCoefs{ 1.f,.045f,.0075f };
        glm::vec3 Color{ 1.f,1.f,1.f };
        glm::vec3 AmbientStrength{ Arcadia::Vec3::Zero() };
        glm::vec3 DiffuseStrength{ 5.f,5.f,5.f };
        glm::vec3 SpecularStrength{ Arcadia::Vec3::Zero() };
    };

    using LightType = std::variant<
        Arcadia::NullLight,
        Arcadia::SpotLight,
        Arcadia::DirectLight,
        Arcadia::AreaLight,
        Arcadia::PointLight
    >;

    struct ARCADIA_API LightComponent;
    struct ARCADIA_API LightComponentMemento
    {
        friend Arcadia::LightComponent;
    public:
        auto operator==(const LightComponentMemento&) const -> bool = default;
    private:
        Arcadia::LightType Light{};
    };

    struct ARCADIA_API LightComponent:
        Arcadia::iComponent,
        Arcadia::iMementoOriginator<Arcadia::LightComponentMemento>
    {
    public:
        using self_type = LightComponent;
    public:
        ARCADIA_COMPONENT_TYPE_STR_GETTERS("light");

        LightComponent() = default;
        LightComponent(const nlohmann::json& json);
        ~LightComponent() = default;
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;

        [[nodiscard]]
        virtual auto OnSnapshot() const->memento_data_type override;
        virtual void OnRestore(const memento_data_type& memento) override;

    public:
        Arcadia::LightType Light{ Arcadia::NullLight{} };
    };
}
