#include "pch.hpp"
#include "camera_component.hpp"

#include"core/math.hpp"

auto arcadia::camera_component::to_json() const -> nlohmann::json
{
    return nlohmann::json{
        { "pos"                     ,arcadia::vec3::to_json(pos) },
        { "target"                  ,arcadia::vec3::to_json(target) },
        { "up"                      ,arcadia::vec3::to_json(up) },
        { "near_plane"              ,near_plane },
        { "far_plane"               ,far_plane },
        { "fovy"                    ,fovy },
        { "fovy_min"                ,fovy_min },
        { "fovy_max"                ,fovy_max },
        { "speed"                   ,speed},
        { "viewport_size"           ,arcadia::ivec2::to_json(viewport_size) },
        { "sensitivity"             ,sensitivity },
        { "fixed_up"                ,fixed_up },
        { "up_epsilon"              ,up_epsilon },
        { "cursor_move_offset_range",arcadia::vec2::to_json(cursor_move_offset_range) }
    };
}

arcadia::camera_component::camera_component(const nlohmann::json& json)
{
    pos                      = arcadia::vec3::from_json(json.at("pos"));
    target                   = arcadia::vec3::from_json(json.at("target"));
    up                       = arcadia::vec3::from_json(json.at("up"));
    near_plane               = json.at("near_plane");
    far_plane                = json.at("far_plane");
    fovy                     = json.at("fovy");
    fovy_min                 = json.at("fovy_min");
    fovy_max                 = json.at("fovy_max");
    speed                    = json.at("speed");
    viewport_size            = arcadia::ivec2::from_json(json.at("viewport_size"));
    sensitivity              = json.at("sensitivity");
    fixed_up                 = json.at("fixed_up");
    up_epsilon               = json.at("up_epsilon");
    cursor_move_offset_range = arcadia::vec2::from_json(json.at("cursor_move_offset_range"));
}

auto arcadia::camera_component::move_forward() -> self_type&
{
    const auto movement = _forward() * speed;
    pos += movement;
    target += movement;
    return *this;
}

auto arcadia::camera_component::move_backward() -> self_type&
{
    const auto movement = _forward() * speed;
    pos -= movement;
    target -= movement;

    return *this;
}

auto arcadia::camera_component::move_left() -> self_type&
{
    const auto movement = glm::cross(_forward(), up) * speed;
    pos -= movement;
    target -= movement;
    return *this;
}

auto arcadia::camera_component::move_right() -> self_type&
{
    const auto movement = glm::cross(_forward(), up) * speed;
    pos += movement;
    target += movement;
    return *this;
}

auto arcadia::camera_component::rotate_view(const glm::vec2& offset) -> self_type&
{
    if(_test_cursor_move(offset.x, offset.y))
    {
        auto forward = _forward();

        //Horizontal
        auto x_angle_offset = -offset.x * sensitivity;
        forward = glm::angleAxis(x_angle_offset, up) * forward;

        //Vertical
        auto y_angle_offset =
            glm::clamp(
                -offset.y * sensitivity + _pitch_angle(),
                -glm::half_pi<float>() + up_epsilon,
                glm::half_pi<float>() - up_epsilon
            )
            - _pitch_angle();
        forward = glm::angleAxis(y_angle_offset, glm::cross(forward, up)) * forward;

        target = pos + forward;
    }
    return *this;
}

auto arcadia::camera_component::drag_view(const glm::vec2& offset) -> self_type&
{
    if(_test_cursor_move(offset.x, offset.y))
    {
        const auto& forward = _forward();

        //Horizontal
        auto x_angle_offset = -offset.x * sensitivity;
        auto rotation = glm::angleAxis(x_angle_offset, up);
        pos = rotation * pos;
        target = rotation * target;

        //Vertical
        auto y_angle_offset =
            glm::clamp(
                -offset.y * sensitivity + _pitch_angle(),
                -glm::half_pi<float>() + up_epsilon,
                glm::half_pi<float>() - up_epsilon
            )
            - _pitch_angle();
        rotation = glm::angleAxis(y_angle_offset, glm::cross(forward, up));
        pos = rotation * pos;
        target = rotation * target;
    }

    return *this;
}

auto arcadia::camera_component::build_view_mat4() const -> glm::mat4
{
    return glm::lookAt(pos, target, up);
}

auto arcadia::camera_component::build_proj_mat4() const -> glm::mat4
{
    return glm::perspective(
        fovy,
        viewport_size.x * 1.f / viewport_size.y,
        near_plane,
        far_plane
    );
}

auto arcadia::camera_component::build_mat4(bool col_major) const -> glm::mat4
{
    if(col_major)
    {
        return build_proj_mat4() * build_view_mat4();
    }
    else
    {
        return glm::transpose(build_view_mat4()) * glm::transpose(build_proj_mat4());
    }
}

auto arcadia::camera_component::_forward() const -> glm::vec3
{
    return glm::normalize(target - pos);
}

auto arcadia::camera_component::_pitch_angle() const -> float
{
    const auto& forward = _forward();
    return glm::angle(forward, up) - glm::half_pi<float>();
}

auto arcadia::camera_component::_yaw_angle() const -> float
{
    const auto& forward = _forward();
    auto yaw_vec = forward - glm::dot(forward, arcadia::vec3::create_pos_unit_y());

    auto coef =
        glm::angle(yaw_vec, arcadia::vec3::create_pos_unit_x()) < glm::half_pi<float>() ?
        1 : -1;
    return coef * glm::angle(yaw_vec, arcadia::vec3::create_neg_unit_z());
}

auto arcadia::camera_component::_roll_angle() const -> float
{
    const auto& forward = _forward();
    auto normal_of_forward_and_up = glm::cross(forward, up);
    auto pos_uni_y_proj_on_forward_and_up = up - glm::dot(up, normal_of_forward_and_up);

    auto coef =
        glm::angle(up, normal_of_forward_and_up) < glm::half_pi<float>() ?
        1 : -1;
    return coef * glm::angle(up, pos_uni_y_proj_on_forward_and_up);
}

auto arcadia::camera_component::_test_cursor_move(float x_offset, float y_offset) -> bool
{

    return arcadia::is_in_range(x_offset, cursor_move_offset_range.x, cursor_move_offset_range.y) &&
        arcadia::is_in_range(y_offset, cursor_move_offset_range.x, cursor_move_offset_range.y);

}
