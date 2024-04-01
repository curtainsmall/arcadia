#pragma once

#include<memory>
#include<variant>

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/uuid.hpp"
#include"platform/jolt/jolt_header.hpp"
#include"resource/component/component.hpp"

namespace arcadia
{
    struct ARCADIA_API jph_box_shape_info
    {
    public:
        glm::vec3 half_extent{ 1.f,1.f,1.f };
        float convex_radius{ JPH::cDefaultConvexRadius };
    };
    struct ARCADIA_API jph_capsule_shape_info
    {
    public:
        float radius{ 1.f };
        float half_height_of_cylinder{ 1.f };
    };
    struct ARCADIA_API jph_cylinder_shape_info
    {
    public:
        float half_height{ 1.f };
        float radius{ 1.f };
        float convex_radius{ JPH::cDefaultConvexRadius };
    };
    struct ARCADIA_API jph_sphere_shape_info
    {
    public:
        float radius{ 1.f };
    };
    using jph_shape_info_type = std::variant<
        jph_box_shape_info,
        jph_capsule_shape_info,
        jph_cylinder_shape_info,
        jph_sphere_shape_info
    >;

    struct ARCADIA_API jph_body_info_initial
    {
    public:
        using self_type = jph_body_info_initial;
    public:
        glm::vec3 position{ arcadia::vec3::zero() };
        glm::quat rotation{ arcadia::quat::identity() };
        JPH::EMotionType jph_motion_type{ JPH::EMotionType::Static };
        JPH::ObjectLayer jph_object_layer{ arcadia::jph_object_layers::non_moving };
        jph_shape_info_type jph_shape_info{ jph_box_shape_info{} };
    };

    struct ARCADIA_API jph_body_info_ongoing
    {
    public:
        using self_type = jph_body_info_ongoing;
    public:
        bool active{ false };
        glm::vec3 position{ arcadia::vec3::zero() };
        glm::quat rotation{ arcadia::quat::identity() };
        glm::vec3 linear_velocity{ arcadia::vec3::zero() };
        glm::vec3 angular_velocity{ arcadia::vec3::zero() };

    };

    struct physics_component;
    struct ARCADIA_API physics_component_memento_data
    {
        friend struct arcadia::physics_component;
    private:
        glm::vec3 body_shape_color{ .2f,.2f,.2f };
        std::unique_ptr<arcadia::jph_body_info_initial> jph_body_info_initial_uptr{};
    };

    struct ARCADIA_API physics_component:
        arcadia::component_base,
        arcadia::memento_originator_interface<arcadia::physics_component_memento_data>
    {
    public:
        using identifiable_jph_body_info_initial_type = arcadia::basic_identifiable<jph_body_info_initial>;
        using self_type = physics_component;
    public:
        ARCADIA_COMPONENT_TYPE_STR_GETTERS("physics");

        physics_component() = default;
        physics_component(const nlohmann::json& json);
        ~physics_component() = default;
        [[nodiscard]]
        auto to_json() const->nlohmann::json;

        [[nodiscard]]
        virtual auto snapshot() const->memento_data_type override;
        virtual void restore(const memento_data_type& memento) override;

        [[nodiscard]]
        auto get_body_shape_color() const -> const glm::vec3&;
        void set_body_shape_color(const glm::vec3& color);

        [[nodiscard]]
        auto has_body_info() const -> bool;

        [[nodiscard]]
        auto get_identifiable_jph_body_info_initial() const -> const identifiable_jph_body_info_initial_type&;

        [[nodiscard]]
        auto get_jph_body_info_ongoing() const -> const arcadia::jph_body_info_ongoing&;
        [[nodiscard]]
        auto get_jph_body_info_ongoing() -> arcadia::jph_body_info_ongoing&;

        void build_identifiable_jph_body_info_initial(
            const glm::vec3& position,
            const glm::quat& rotation,
            JPH::EMotionType jph_motion_type,
            JPH::ObjectLayer jph_object_layer,
            const jph_shape_info_type& jph_shape_info
        );

        void build_identifiable_jph_body_info_initial(
            const arcadia::jph_body_info_initial& jph_body_info_initial
        );

    private:
        glm::vec3 _body_shape_color{ .2f,.2f,.2f };
        std::unique_ptr<identifiable_jph_body_info_initial_type> _identifiable_jph_body_info_initial_uptr{};
        std::unique_ptr<arcadia::jph_body_info_ongoing> _jph_body_info_ongoing_uptr{};
    };
}
