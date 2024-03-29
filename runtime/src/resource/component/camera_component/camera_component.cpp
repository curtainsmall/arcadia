#include "pch.hpp"
#include "camera_component.hpp"

#include"core/math.hpp"

auto arcadia::camera_component::to_json() const -> nlohmann::json
{
    return nlohmann::json{
        { "position"                ,arcadia::vec3::to_json(_position) },
        { "target"                  ,arcadia::vec3::to_json(_target) },
        { "up"                      ,arcadia::vec3::to_json(_up) },
        { "near_plane"              ,_near_plane },
        { "far_plane"               ,_far_plane },
        { "fov"                     ,_fov },
        { "fov_min"                 ,_fov_min },
        { "fov_max"                 ,_fov_max },
        { "speed"                   ,_speed},
        { "viewport_size"           ,arcadia::ivec2::to_json(_viewport_size) },
        { "fixed_up"                ,_fixed_up },
        { "up_epsilon"              ,_up_epsilon },
        { "cursor_move_offset_range",arcadia::vec2::to_json(_cursor_move_offset_range) }
    };
}

auto arcadia::camera_component::snapshot() const -> memento_data_type
{
    memento_data_type memento{};

    memento.position      = _position;
    memento.target        = _target;
    memento.up            = _up;
    memento.near_plane    = _near_plane;
    memento.far_plane     = _far_plane;
    memento.fov           = _fov;
    memento.fov_min       = _fov_min;
    memento.fov_max       = _fov_max;
    memento.speed         = _speed;
    memento.viewport_size = _viewport_size;
    memento.fixed_up      = _fixed_up;
    memento.up_epsilon    = _up_epsilon;

    return memento;
}

void arcadia::camera_component::restore(const memento_data_type& memento)
{
    _position      = memento.position;
    _target        = memento.target;
    _up            = memento.up;
    _near_plane    = memento.near_plane;
    _far_plane     = memento.far_plane;
    _fov           = memento.fov;
    _fov_min       = memento.fov_min;
    _fov_max       = memento.fov_max;
    _speed         = memento.speed;
    _viewport_size = memento.viewport_size;
    _fixed_up      = memento.fixed_up;
    _up_epsilon    = memento.up_epsilon;

}

auto arcadia::camera_component::get_position() const -> const glm::vec3&
{
    return _position;
}

void arcadia::camera_component::set_position(const glm::vec3& position)
{
    _position = position;
}

auto arcadia::camera_component::get_target() const -> const glm::vec3&
{
    return _target;
}

void arcadia::camera_component::set_target(const glm::vec3& target)
{
    _target = target;
}

auto arcadia::camera_component::get_up() const -> const glm::vec3&
{
    return _up;
}

void arcadia::camera_component::set_up(const glm::vec3& up)
{
    _up = up;
}

auto arcadia::camera_component::get_near_plane() const -> float
{
    return _near_plane;
}

void arcadia::camera_component::set_near_plane(float near_plane)
{
    _near_plane = near_plane;
}

auto arcadia::camera_component::get_far_plane() const -> float
{
    return _far_plane;
}

void arcadia::camera_component::set_far_plane(float far_plane)
{
    _far_plane = far_plane;
}

auto arcadia::camera_component::get_fov() const -> float
{
    return _fov;
}

void arcadia::camera_component::set_fov(float fov)
{
    _fov = fov;
}

auto arcadia::camera_component::get_fov_min() const -> float
{
    return _fov_min;
}

void arcadia::camera_component::set_fov_min(float fov_min)
{
    _fov_min = fov_min;
}

auto arcadia::camera_component::get_fov_max() const -> float
{
    return _fov_max;
}

void arcadia::camera_component::set_fov_max(float fov_max)
{
    _fov_max = fov_max;
}

auto arcadia::camera_component::get_speed() const -> float
{
    return _speed;
}

void arcadia::camera_component::set_speed(float speed)
{
    _speed = speed;
}

auto arcadia::camera_component::get_viewport_size() const -> const glm::ivec2&
{
    return _viewport_size;
}

void arcadia::camera_component::set_viewport_size(const glm::ivec2& viewport_size)
{
    _viewport_size = viewport_size;
}

auto arcadia::camera_component::get_fixed_up() const -> bool
{
    return _fixed_up;
}

void arcadia::camera_component::set_fixed_up(bool fixed_up)
{
    _fixed_up = fixed_up;
}

auto arcadia::camera_component::get_up_epsilon() const -> float
{
    return _up_epsilon;
}

void arcadia::camera_component::set_up_epsilon(float up_epsilon)
{
    _up_epsilon = up_epsilon;
}

auto arcadia::camera_component::get_cursor_move_offset_range() const -> const glm::vec2&
{
    return _cursor_move_offset_range;
}

void arcadia::camera_component::set_cursor_move_offset_range(const glm::vec2& cursor_move_offset_range)
{
    _cursor_move_offset_range = cursor_move_offset_range;
}

auto arcadia::camera_component::get_should_display_grid() const -> bool
{
    return _should_display_grid;
}

void arcadia::camera_component::set_should_display_grid(bool should_display_grid)
{
    _should_display_grid = should_display_grid;
}

arcadia::camera_component::camera_component(const nlohmann::json& json)
{
    _position                 = arcadia::vec3::from_json(json.at("position"));
    _target                   = arcadia::vec3::from_json(json.at("target"));
    _up                       = arcadia::vec3::from_json(json.at("up"));
    _near_plane               = json.at("near_plane");
    _far_plane                = json.at("far_plane");
    _fov                      = json.at("fov");
    _fov_min                  = json.at("fov_min");
    _fov_max                  = json.at("fov_max");
    _speed                    = json.at("speed");
    _viewport_size            = arcadia::ivec2::from_json(json.at("viewport_size"));
    _fixed_up                 = json.at("fixed_up");
    _up_epsilon               = json.at("up_epsilon");
    _cursor_move_offset_range = arcadia::vec2::from_json(json.at("cursor_move_offset_range"));
}

auto arcadia::camera_component::move_forward() -> self_type&
{
    const auto movement = get_forward_dir() * _speed;
    _position += movement;
    _target += movement;
    return *this;
}

auto arcadia::camera_component::move_backward() -> self_type&
{
    const auto movement = get_forward_dir() * _speed;
    _position -= movement;
    _target -= movement;
    return *this;
}

auto arcadia::camera_component::move_left() -> self_type&
{
    const auto movement = glm::cross(get_forward_dir(), _up) * _speed;
    _position -= movement;
    _target -= movement;
    return *this;
}

auto arcadia::camera_component::move_right() -> self_type&
{
    const auto movement = glm::cross(get_forward_dir(), _up) * _speed;
    _position += movement;
    _target += movement;
    return *this;
}

auto arcadia::camera_component::move(const glm::vec3& offset) -> self_type&
{
    _position += offset;
    _target += offset;
    return *this;
}

auto arcadia::camera_component::drag_view_move(const glm::vec2& offset) -> self_type&
{
    move(
        get_left_dir() * offset.x
        + get_up_dir() * offset.y
    );
    return *this;
}

auto arcadia::camera_component::rotate_view(const glm::vec2& offset) -> self_type&
{
    if(_test_cursor_move(offset.x, offset.y))
    {
        auto forward = get_forward_dir();

        //Horizontal
        auto x_angle_offset = -offset.x;
        forward = glm::angleAxis(x_angle_offset, _up) * forward;

        //Vertical
        auto y_angle_offset =
            glm::clamp(
                -offset.y + _pitch_angle(),
                -glm::half_pi<float>() + _up_epsilon,
                glm::half_pi<float>() - _up_epsilon
            )
            - _pitch_angle();
        forward = glm::angleAxis(y_angle_offset, glm::cross(forward, _up)) * forward;

        _target = _position + forward;
    }
    return *this;
}

auto arcadia::camera_component::drag_view_rotate(const glm::vec2& offset) -> self_type&
{
    if(_test_cursor_move(offset.x, offset.y))
    {
        const auto& forward = get_forward_dir();

        //Horizontal
        auto x_angle_offset = -offset.x;
        auto rotation = glm::angleAxis(x_angle_offset, _up);
        _position = rotation * _position;
        _target = rotation * _target;

        //Vertical
        auto y_angle_offset =
            glm::clamp(
                -offset.y + _pitch_angle(),
                -glm::half_pi<float>() + _up_epsilon,
                glm::half_pi<float>() - _up_epsilon
            )
            - _pitch_angle();
        rotation = glm::angleAxis(y_angle_offset, glm::cross(forward, _up));
        _position = rotation * _position;
        _target = rotation * _target;
    }

    return *this;
}

auto arcadia::camera_component::gen_view_mat4() const -> glm::mat4
{
    return glm::lookAt(_position, _target, _up);
}

auto arcadia::camera_component::gen_proj_mat4() const -> glm::mat4
{
    return glm::perspective(
        _fov,
        _viewport_size.x * 1.f / _viewport_size.y,
        _near_plane,
        _far_plane
    );
}

auto arcadia::camera_component::gen_mat4(bool col_major) const -> glm::mat4
{
    if(col_major)
    {
        return gen_proj_mat4() * gen_view_mat4();
    }
    else
    {
        return glm::transpose(gen_view_mat4()) * glm::transpose(gen_proj_mat4());
    }
}

auto arcadia::camera_component::get_forward_dir() const -> glm::vec3
{
    return glm::normalize(_target - _position);
}

auto arcadia::camera_component::get_left_dir() const -> glm::vec3
{
    return glm::normalize(glm::cross(_up, get_forward_dir()));
}

auto arcadia::camera_component::get_up_dir() const -> glm::vec3
{
    return glm::normalize(glm::cross(get_forward_dir(), get_left_dir()));
}

auto arcadia::camera_component::_pitch_angle() const -> float
{
    const auto& forward = get_forward_dir();
    return glm::angle(forward, _up) - glm::half_pi<float>();
}

auto arcadia::camera_component::_yaw_angle() const -> float
{
    const auto& forward = get_forward_dir();
    auto yaw_vec = forward - glm::dot(forward, arcadia::vec3::pos_unit_y());

    auto coef =
        glm::angle(yaw_vec, arcadia::vec3::pos_unit_x()) < glm::half_pi<float>() ?
        1 : -1;
    return coef * glm::angle(yaw_vec, arcadia::vec3::neg_unit_z());
}

auto arcadia::camera_component::_roll_angle() const -> float
{
    const auto& forward = get_forward_dir();
    auto normal_of_forward_and_up = glm::cross(forward, _up);
    auto pos_uni_y_proj_on_forward_and_up = _up - glm::dot(_up, normal_of_forward_and_up);

    auto coef =
        glm::angle(_up, normal_of_forward_and_up) < glm::half_pi<float>() ?
        1 : -1;
    return coef * glm::angle(_up, pos_uni_y_proj_on_forward_and_up);
}

auto arcadia::camera_component::_test_cursor_move(float x_offset, float y_offset) -> bool
{

    return arcadia::is_in_range(x_offset, _cursor_move_offset_range.x, _cursor_move_offset_range.y) &&
        arcadia::is_in_range(y_offset, _cursor_move_offset_range.x, _cursor_move_offset_range.y);

}
