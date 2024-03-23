#pragma once

#include<memory>
#include<variant>

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/uuid.hpp"
#include"platform/jolt/jolt_header.hpp"
#include"resource/component/component.hpp"

namespace arcadia
{
    struct ARCADIA_API physics_component: arcadia::component_base
    {
    public:
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

        using identifiable_jph_body_info_initial = arcadia::basic_identifiable<jph_body_info_initial>;
        using self_type = physics_component;
    public:
        ARCADIA_COMPONENT_TYPE_STR_GETTERS("physics");

        physics_component() = default;
        physics_component(const nlohmann::json& json);
        ~physics_component() = default;
        [[nodiscard]]
        auto to_json() const->nlohmann::json;

        [[nodiscard]]
        inline auto has_body_info() const -> bool
        {
            return _identifiable_jph_body_info_initial_uptr.get();
        }
        [[nodiscard]]
        inline auto get_identifiable_jph_body_info_initial() const -> const identifiable_jph_body_info_initial&
        {
            ARCADIA_ASSERT(has_body_info());
            return *_identifiable_jph_body_info_initial_uptr;
        }
        [[nodiscard]]
        inline auto get_jph_body_info_ongoing() const -> const jph_body_info_ongoing&
        {
            ARCADIA_ASSERT(has_body_info());
            return *_jph_body_info_ongoing_uptr;
        }
        [[nodiscard]]
        inline auto get_jph_body_info_ongoing() -> jph_body_info_ongoing&
        {
            ARCADIA_ASSERT(has_body_info());
            return *_jph_body_info_ongoing_uptr;
        }

        void build_identifiable_jph_body_info(
            const glm::vec3& position,
            const glm::quat& rotation,
            JPH::EMotionType jph_motion_type,
            JPH::ObjectLayer jph_object_layer,
            const jph_shape_info_type& jph_shape_info
        );


    private:
        std::unique_ptr<identifiable_jph_body_info_initial> _identifiable_jph_body_info_initial_uptr{};
        std::unique_ptr<jph_body_info_ongoing> _jph_body_info_ongoing_uptr{};
    };
}
