#pragma once

#include<memory>
#include<variant>

#include"core/math.hpp"
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

        [[nodiscard]]
        auto GetAttenuationCoefficients() const -> const glm::vec3&;
        void SetAttenuationCoefficients(const glm::vec3& coeffs);

        [[nodiscard]]
        auto GetCutoffAngles() const -> const glm::vec2&;
        void SetCutoffAngles(const glm::vec2& angles);

        [[nodiscard]]
        auto GetColor() const -> const glm::vec3&;
        void SetColor(const glm::vec3& color);

        [[nodiscard]]
        auto GetAmbientStrength() const -> const glm::vec3&;
        void SetAmbientStrength(const glm::vec3& strength);

        [[nodiscard]]
        auto GetDiffuseStrength() const -> const glm::vec3;
        void SetDiffuseStrength(const glm::vec3& strength);

        [[nodiscard]]
        auto GetSpecularStrength() const -> const glm::vec3&;
        void SetSpecularStrength(const glm::vec3& strength);


    private:
        glm::vec3 _AttenuationCoefficients{ 1.f,.045f,.0075f };
        glm::vec2 _CutoffAngles{ glm::radians(30.f),glm::radians(32.f) };
        glm::vec3 _Color{ 1.f,1.f,1.f };
        glm::vec3 _AmbientStrength{ GlmVec3::CreateZero() };
        glm::vec3 _DiffuseStrength{ 5.f,5.f,5.f };
        glm::vec3 _SpecularStrength{ GlmVec3::CreateZero() };
    };

    class DirectLight
    {
    public:
        auto operator==(const DirectLight&) const -> bool = default;

        [[nodiscard]]
        auto GetColor() const -> const glm::vec3&;
        void SetColor(const glm::vec3& color);

        [[nodiscard]]
        auto GetAmbientStrength() const -> const glm::vec3&;
        void SetAmbientStrength(const glm::vec3& strength);

        [[nodiscard]]
        auto GetDiffuseStrength() const -> const glm::vec3;
        void SetDiffuseStrength(const glm::vec3& strength);

        [[nodiscard]]
        auto GetSpecularStrength() const -> const glm::vec3&;
        void SetSpecularStrength(const glm::vec3& strength);
    private:
        glm::vec3 _Color{ 1.f,1.f,1.f };
        glm::vec3 _AmbientStrength{ GlmVec3::CreateZero() };
        glm::vec3 _DiffuseStrength{ 5.f,5.f,5.f };
        glm::vec3 _SpecularStrength{ GlmVec3::CreateZero() };
    };

    class AreaLight
    {
    public:
        auto operator==(const AreaLight&) const -> bool = default;

        [[nodiscard]]
        auto GetSize() const -> const glm::vec2&;
        void SetSize(const glm::vec2& size);

        [[nodiscard]]
        auto GetColor() const -> const glm::vec3&;
        void SetColor(const glm::vec3& color);

        [[nodiscard]]
        auto GetAmbientStrength() const -> const glm::vec3&;
        void SetAmbientStrength(const glm::vec3& strength);

        [[nodiscard]]
        auto GetDiffuseStrength() const -> const glm::vec3;
        void SetDiffuseStrength(const glm::vec3& strength);

        [[nodiscard]]
        auto GetSpecularStrength() const -> const glm::vec3&;
        void SetSpecularStrength(const glm::vec3& strength);

    private:
        glm::vec2 _Size{ GlmVec2::CreateZero() };
        glm::vec3 _Color{ 1.f,1.f,1.f };
        glm::vec3 _AmbientStrength{ GlmVec3::CreateZero() };
        glm::vec3 _DiffuseStrength{ 5.f,5.f,5.f };
        glm::vec3 _SpecularStrength{ GlmVec3::CreateZero() };
    };

    class PointLight
    {
    public:
        auto operator==(const PointLight&) const -> bool = default;

        [[nodiscard]]
        auto GetAttenuationCoefficients() const -> const glm::vec3&;
        void SetAttenuationCoefficients(const glm::vec3& coeffs);

        [[nodiscard]]
        auto GetColor() const -> const glm::vec3&;
        void SetColor(const glm::vec3& color);

        [[nodiscard]]
        auto GetAmbientStrength() const -> const glm::vec3&;
        void SetAmbientStrength(const glm::vec3& strength);

        [[nodiscard]]
        auto GetDiffuseStrength() const -> const glm::vec3;
        void SetDiffuseStrength(const glm::vec3& strength);

        [[nodiscard]]
        auto GetSpecularStrength() const -> const glm::vec3&;
        void SetSpecularStrength(const glm::vec3& strength);
    private:
        glm::vec3 _AttenuationCoefficients{ 1.f,.045f,.0075f };
        glm::vec3 _Color{ 1.f,1.f,1.f };
        glm::vec3 _AmbientStrength{ GlmVec3::CreateZero() };
        glm::vec3 _DiffuseStrength{ 5.f,5.f,5.f };
        glm::vec3 _SpecularStrength{ GlmVec3::CreateZero() };
    };

    using LightType = std::variant<
        NullLight,
        SpotLight,
        DirectLight,
        AreaLight,
        PointLight
    >;

    class LightComponent:
        public ComponentInterface
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

        [[nodiscard]]
        auto GetLight() const -> const LightType&;
        [[nodiscard]]
        auto GetLight() -> LightType&;
        void SetLight(const LightType& light);

        template<typename Light, typename ...Args>
        void SetLight(Args&& ...args)
        {
            _Light = Light(std::forward<Args>(args)...);
        }

    private:
        LightType _Light{ NullLight{} };
    };
}