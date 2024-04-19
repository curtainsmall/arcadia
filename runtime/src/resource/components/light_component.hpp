#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"resource/components/component_interface.hpp"

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

    struct ARCADIA_API LightComponentMementoData: Arcadia::MementoDataBase
    {
    public:
        auto operator==(const LightComponentMementoData&) const -> bool = default;
    public:
        Arcadia::LightType Light{};
    };

    struct ARCADIA_API LightComponent:
        Arcadia::iComponent,
        Arcadia::iMementoOriginator
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

    protected:
        [[nodiscard]]
        virtual auto OnSnapshot() const->std::shared_ptr<Arcadia::MementoDataBase> override;
        virtual void OnRestore(const std::shared_ptr<Arcadia::MementoDataBase>& sp_memento_data) override;

    public:
        Arcadia::LightType Light{ Arcadia::NullLight{} };
    };
}
