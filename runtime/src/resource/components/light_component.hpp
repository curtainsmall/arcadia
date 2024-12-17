#pragma once

#include<memory>
#include<variant>

#include"core/math.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"platform/api_def.hpp"
#include"resource/components/component_interface.hpp"

namespace Arcadia
{
    class NullLight
    {
    public:
        auto operator==(const NullLight&) const -> bool = default;
    };

    class SpotLight
    {
    public:
        auto operator==(const SpotLight&) const -> bool = default;
    public:
        glm::vec3 AttenuationCoefficients{ 1.f,.045f,.0075f };
        glm::vec2 CutoffAngles{ glm::radians(30.f),glm::radians(32.f) };
        glm::vec3 Color{ 1.f,1.f,1.f };
        glm::vec3 AmbientStrength{ GlmVec3::CreateZero() };
        glm::vec3 DiffuseStrength{ 5.f,5.f,5.f };
        glm::vec3 SepcularStrength{ GlmVec3::CreateZero() };
    };

    class DirectLight
    {
    public:
        auto operator==(const DirectLight&) const -> bool = default;
    public:
        glm::vec3 Color{ 1.f,1.f,1.f };
        glm::vec3 AmbientStrength{ GlmVec3::CreateZero() };
        glm::vec3 DiffuseStrength{ 5.f,5.f,5.f };
        glm::vec3 SepcularStrength{ GlmVec3::CreateZero() };
    };

    class AreaLight
    {
    public:
        auto operator==(const AreaLight&) const -> bool = default;
    public:
        glm::vec2 Size{ GlmVec2::CreateZero() };
        glm::vec3 Color{ 1.f,1.f,1.f };
        glm::vec3 AmbientStrength{ GlmVec3::CreateZero() };
        glm::vec3 DiffuseStrength{ 5.f,5.f,5.f };
        glm::vec3 SepcularStrength{ GlmVec3::CreateZero() };
    };

    class PointLight
    {
    public:
        auto operator==(const PointLight&) const -> bool = default;
    public:
        glm::vec3 AttenuationCoefficients{ 1.f,.045f,.0075f };
        glm::vec3 Color{ 1.f,1.f,1.f };
        glm::vec3 AmbientStrength{ GlmVec3::CreateZero() };
        glm::vec3 DiffuseStrength{ 5.f,5.f,5.f };
        glm::vec3 SepcularStrength{ GlmVec3::CreateZero() };
    };

    using LightType = std::variant<
        NullLight,
        SpotLight,
        DirectLight,
        AreaLight,
        PointLight
    >;

    class LightComponentMementoData: public MementoDataBase
    {
    public:
        auto operator==(const LightComponentMementoData&) const -> bool = default;
    public:
        LightType Light{};
    };

    class LightComponent:
        public iComponent,
        public iMementoOriginator
    {
    public:
        using SelfType = LightComponent;
    public:
        ACDA_COMPONENT_TYPE_STR_GETTERS("light");

        LightComponent() = default;
        LightComponent(const nlohmann::json& json);
        ~LightComponent() = default;
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;

    protected:
        [[nodiscard]]
        virtual auto OnSnapshot() const->std::shared_ptr<MementoDataBase> override;
        virtual void OnRestore(const std::shared_ptr<MementoDataBase>& sp_memento_data) override;

    public:
        LightType Light{ NullLight{} };
    };
}