#pragma once

#include"flatbuffers_generated/asset_generated.h"

#include"core/base.hpp"
#include"core/math.hpp"
#include"core/nlohmann_json_header.hpp"
#include"resource/component/component.hpp"

namespace arcadia
{
    struct ARCADIA_API camera_component: arcadia::component_base
    {
    public:
        using self_type = arcadia::camera_component;
        using serialization_type = arcadia::serialization::camera;
    public:
        ARCADIA_COMPONENT_TYPE_STR_GETERS("Camera");

        camera_component() = default;
        camera_component(const nlohmann::json& json);
        ~camera_component() = default;
        auto to_json() const->nlohmann::json;

        auto move_forward() -> self_type&;
        auto move_backward() -> self_type&;
        auto move_left() -> self_type&;
        auto move_right() -> self_type&;
        auto move(const glm::vec3& offset) -> self_type&;
        auto drag_view_move(const glm::vec2& offset) -> self_type&;

        auto rotate_view(const glm::vec2& offset) -> self_type&;
        auto drag_view_rotate(const glm::vec2& offset) -> self_type&;

        [[nodiscard]]
        auto build_view_mat4() const->glm::mat4;

        [[nodiscard]]
        auto build_proj_mat4() const->glm::mat4;

        [[nodiscard]]
        auto build_mat4(bool col_major = true) const->glm::mat4;

        // Get forward vector by position and target
        auto get_forward_dir() const->glm::vec3;
        auto get_left_dir() const->glm::vec3;
        auto get_up_dir() const->glm::vec3;
    private:

        // Angle of pitch
        // Look from right:
        // increase when rotating counter-clockwise
        auto _pitch_angle() const -> float;

        // Angle of yaw
        // Look from top:
        // increase when rotating counter-clockwise
        auto _yaw_angle() const->float;

        // Angle of roll
        // Lock from back:
        // increase when rotating counter-clockwise
        auto _roll_angle() const->float;

        // Test whether a cursor move should be filtered
        auto _test_cursor_move(float x_offset, float y_offset) -> bool;

    public:
        /// @brief Position of camera
        glm::vec3 pos{ arcadia::vec3::pos_unit_z() };

        /// @brief Target that the camera pointing to
        glm::vec3 target{ arcadia::vec3::zero() };

        /// @brief Direction of global up
        /// @note This should always be unit vector
        glm::vec3 up{ arcadia::vec3::pos_unit_y() };

        /// @brief Near plane of clip space
        float near_plane{ .1f };

        /// @brief Far plane of clip space
        float far_plane{ 100.f };

        /// @brief FOV angle in vertical direction
        float fov{ 90.f };

        /// @brief Minimun value fo @ref arcadia::camera::fov
        float fov_min{ 1.f };

        /// @brief Maximun value of @ref arcadia::camera::fov
        float fov_max{ 120.f };

        /// @brief Move speed of free-camera
        float speed{ .25f };

        /// @brief Size of the viewport of this camera
        glm::ivec2 viewport_size{ 800,600 };

        /// @brief Whether @ref arcadia::camera::up should be fixed
        bool fixed_up{ true };

        /// @brief How small angle between @ref arcadia::camera::up and @ref arcadia::camera::target can be
        float up_epsilon{ .1f };

        /// @brief Cursor move offset that is out of this range will be silently ignored
        glm::vec2 cursor_move_offset_range{ -100.f,100.f };

        /// @brief Display a grid a X-Z plane
        bool should_display_grid{ false };
    };
}
