#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"resource/component/component.hpp"

namespace arcadia
{
    struct ARCADIA_API null_light
    {};

    struct ARCADIA_API spot_light
    {
    public:
        glm::vec3 position{ arcadia::vec3::zero() };
        glm::vec3 direction{ arcadia::vec3::zero() };
        glm::vec3 attenuation_coefs{ 1.f,.045f,.0075f };
        glm::vec2 cutoff_angles{ glm::radians(30.f),glm::radians(32.f) };
        glm::vec3 color{ 1.f,1.f,1.f };
        glm::vec3 ambient_strength{ arcadia::vec3::zero() };
        glm::vec3 diffuse_strength{ 5.f,5.f,5.f };
        glm::vec3 specular_strength{ arcadia::vec3::zero() };
    };

    struct ARCADIA_API direct_light
    {
    public:
        glm::vec3 direction{ arcadia::vec3::zero() };
        glm::vec3 color{ 1.f,1.f,1.f };
        glm::vec3 ambient_strength{ arcadia::vec3::zero() };
        glm::vec3 diffuse_strength{ 5.f,5.f,5.f };
        glm::vec3 specular_strength{ arcadia::vec3::zero() };
    };

    struct ARCADIA_API area_light
    {
    public:
        glm::vec3 position{ arcadia::vec3::zero() };
        glm::vec3 direction{ arcadia::vec3::zero() };
        glm::vec2 size{ arcadia::vec2::zero() };
        glm::vec3 color{ 1.f,1.f,1.f };
        glm::vec3 ambient_strength{ arcadia::vec3::zero() };
        glm::vec3 diffuse_strength{ 5.f,5.f,5.f };
        glm::vec3 specular_strength{ arcadia::vec3::zero() };
    };

    struct ARCADIA_API point_light
    {
    public:
        glm::vec3 position{ arcadia::vec3::zero() };
        glm::vec3 attenuation_coefs{ 1.f,.045f,.0075f };
        glm::vec3 color{ 1.f,1.f,1.f };
        glm::vec3 ambient_strength{ arcadia::vec3::zero() };
        glm::vec3 diffuse_strength{ 5.f,5.f,5.f };
        glm::vec3 specular_strength{ arcadia::vec3::zero() };
    };

    using light_type = std::variant<
        arcadia::null_light,
        arcadia::spot_light,
        arcadia::direct_light,
        arcadia::area_light,
        arcadia::point_light
    >;

    struct ARCADIA_API light_component;
    struct ARCADIA_API light_component_memento
    {
        friend arcadia::light_component;
    private:
        arcadia::light_type light{};
    };

    struct ARCADIA_API light_component:
        arcadia::component_base,
        arcadia::memento_originator_interface<arcadia::light_component_memento>
    {
    public:
        using self_type = light_component;
    public:
        ARCADIA_COMPONENT_TYPE_STR_GETTERS("light");

        light_component() = default;
        light_component(const nlohmann::json& json);
        ~light_component() = default;
        [[nodiscard]]
        auto to_json() const->nlohmann::json;

        [[nodiscard]]
        virtual auto snapshot() const->memento_data_type override;
        virtual void restore(const memento_data_type& memento) override;

    public:
        arcadia::light_type light{ arcadia::null_light{} };
    };
}
