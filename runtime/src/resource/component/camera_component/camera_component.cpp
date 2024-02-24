#include "pch.hpp"
#include "camera_component.hpp"

#include"core/math.hpp"

auto arcadia::camera_component::to_flatbuffers(flatbuffers::FlatBufferBuilder& builder, const self_type& camera_component) -> flatbuffers::Offset<serialization_type>
{
    auto pos = arcadia::vec3::to_flatbuffers(camera_component.pos);
    auto target = arcadia::vec3::to_flatbuffers(camera_component.target);
    auto up = arcadia::vec3::to_flatbuffers(camera_component.up);
    auto viewport_size = arcadia::uvec2::to_flatbuffers(camera_component.viewport_size);
    auto cursor_move_offset_range = arcadia::vec2::to_flatbuffers(camera_component.cursor_move_offset_range);

    return arcadia::serialization::Createcamera(
        builder,
        builder.CreateSharedString(camera_component._name),
        &pos,
        &target,
        &up,
        camera_component.near_plane,
        camera_component.far_plane,
        camera_component.fovy,
        camera_component.fovy_min,
        camera_component.fovy_max,
        camera_component.speed,
        &viewport_size,
        camera_component.fixed_up,
        camera_component.up_epsilon,
        &cursor_move_offset_range
    );
}

auto arcadia::camera_component::from_flatbuffers(const serialization_type& flat_camera) -> self_type
{
    arcadia::camera_component camera_component{ flat_camera.name()->str() };

    camera_component.pos = arcadia::vec3::from_flatbuffers(*flat_camera.pos());
    camera_component.target = arcadia::vec3::from_flatbuffers(*flat_camera.target());
    camera_component.up = arcadia::vec3::from_flatbuffers(*flat_camera.up());
    camera_component.near_plane = flat_camera.near_plane();
    camera_component.far_plane = flat_camera.far_plane();
    camera_component.fovy = flat_camera.fovy();
    camera_component.fovy_min = flat_camera.fovy_min();
    camera_component.fovy_max = flat_camera.fovy_max();
    camera_component.speed = flat_camera.speed();
    camera_component.viewport_size = arcadia::uvec2::from_flatbuffers(*flat_camera.viewport_size());
    camera_component.fixed_up = flat_camera.fixed_up();
    camera_component.up_epsilon = flat_camera.up_epsilon();
    camera_component.cursor_move_offset_range = arcadia::vec2::from_flatbuffers(*flat_camera.cursor_move_offset_range());

    return camera_component;
}

arcadia::camera_component::camera_component(const std::string& name):
    _name(name)
{}

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
                -glm::half_pi<float>() + up_epsilon.get_rad(),
                glm::half_pi<float>() - up_epsilon.get_rad()
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
                -glm::half_pi<float>() + up_epsilon.get_rad(),
                glm::half_pi<float>() - up_epsilon.get_rad()
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
        fovy.get_rad(),
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
