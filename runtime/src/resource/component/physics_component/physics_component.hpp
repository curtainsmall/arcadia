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
            JPH::Vec3 half_extent{ JPH::Vec3::sZero() };
            float convex_radius{ .0f };
        };
        struct ARCADIA_API jph_capsule_shape_info
        {
        public:
            float radius{ .0f };
            float half_height_of_cylinder{ .0f };
        };
        struct ARCADIA_API jph_cylinder_shape_info
        {
        public:
            float half_height{ .0f };
            float radius{ .0f };
            float convex_radius{ .0f };
        };
        struct ARCADIA_API jph_sphere_shape_info
        {
        public:
            float radius{ .0f };
        };
        using jph_shape_info_type = std::variant<
            jph_box_shape_info,
            jph_capsule_shape_info,
            jph_cylinder_shape_info,
            jph_sphere_shape_info
        >;

        struct ARCADIA_API jph_body_info
        {
        public:
            using self_type = jph_body_info;
        public:
            JPH::RVec3Arg jph_position{ JPH::RVec3::sZero() };
            JPH::QuatArg jph_rotation{ JPH::Quat::sIdentity() };
            JPH::EMotionType jph_motion_type{ JPH::EMotionType::Static };
            JPH::ObjectLayer jph_object_layer{ arcadia::jph_object_layers::non_moving };
            jph_shape_info_type jph_shape_info{ jph_box_shape_info{} };
        };

        using identifiable_jph_body_info = arcadia::basic_identifiable<jph_body_info>;
        using self_type = physics_component;
    public:
        ARCADIA_COMPONENT_TYPE_STR_GETTERS("physics");

        physics_component() = default;
        physics_component(const nlohmann::json& json);
        ~physics_component() = default;
        [[nodiscard]]
        auto to_json() const->nlohmann::json;

        [[nodiscard]]
        inline auto has_identifiable_jph_body_info() const -> bool
        {
            return _identifiable_jph_body_info_uptr.get();
        }
        [[nodiscard]]
        inline auto get_identifiable_jph_body_info() const -> const identifiable_jph_body_info&
        {
            return *_identifiable_jph_body_info_uptr;
        }

        inline void build_identifiable_jph_body_info(
            const JPH::RVec3Arg& jph_position,
            const JPH::QuatArg& jph_rotation,
            JPH::EMotionType jph_motion_type,
            JPH::ObjectLayer jph_object_layer,
            jph_shape_info_type&& jph_shape_info
        )
        {
            _identifiable_jph_body_info_uptr = std::make_unique<identifiable_jph_body_info>(
                jph_position,
                jph_rotation,
                jph_motion_type,
                jph_object_layer,
                jph_shape_info_type{ std::move(jph_shape_info) }
            );
        }

    private:
        std::unique_ptr<identifiable_jph_body_info> _identifiable_jph_body_info_uptr{};
    };
}
