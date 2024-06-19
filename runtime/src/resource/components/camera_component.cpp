#include "pch.hpp"

#include "camera_component.hpp"

#include"core/math.hpp"

CameraComponent::CameraComponent(const nlohmann::json& json)
{
    near_plane             = json.at("near_plane");
    far_plane              = json.at("far_plane");
    fovy                  = json.at("fovy");
    fovy_min               = json.at("fovy_min");
    fovy_max               = json.at("fovy_max");
    speed                 = json.at("speed");
    viewport_size          = ivec2::from_json(json.at("viewport_size"));
    fixed_up               = json.at("fixed_up");
    up_epsilon             = json.at("up_epsilon");
    cursor_move_offset_range = vec2::from_json(json.at("cursor_move_offset_range"));
}

auto CameraComponent::to_json() const -> nlohmann::json
{
    return nlohmann::json{
        { "near_plane"              ,near_plane },
        { "far_plane"               ,far_plane },
        { "fovy"                    ,fovy },
        { "fovy_min"                ,fovy_min },
        { "fovy_max"                ,fovy_max },
        { "speed"                   ,speed},
        { "viewport_size"           ,ivec2::to_json(viewport_size) },
        { "fixed_up"                ,fixed_up },
        { "up_epsilon"              ,up_epsilon },
        { "cursor_move_offset_range",vec2::to_json(cursor_move_offset_range) }
    };
}

auto CameraComponent::generate_view_mat4(const glm::vec3& pos, const glm::vec3& dir) -> glm::mat4
{
    return glm::lookAt(pos, pos + dir, up);
}

auto CameraComponent::generate_proj_mat4() const -> glm::mat4
{
    return glm::perspective(
        fovy,
        viewport_size.x * 1.f / viewport_size.y,
        near_plane,
        far_plane
    );
}

auto CameraComponent::on_snapshot() const -> std::shared_ptr<MementoDataBase>
{
    auto sp_memento = std::make_shared<CameraComponentMementoData>();

    sp_memento->near_plane             = near_plane;
    sp_memento->far_plane              = far_plane;
    sp_memento->fovy                   = fovy;
    sp_memento->fovy_min                = fovy_min;
    sp_memento->fovy_max                = fovy_max;
    sp_memento->speed                 = speed;
    sp_memento->viewport_size          = viewport_size;
    sp_memento->fixed_up               = fixed_up;
    sp_memento->up_epsilon             = up_epsilon;
    sp_memento->cursor_move_offset_range = cursor_move_offset_range;
    sp_memento->should_display_grid     = should_display_grid;

    return sp_memento;
}

void CameraComponent::on_restore(const std::shared_ptr<MementoDataBase>& sp_memento_data)
{
    auto& memento_data = sp_memento_data->as<CameraComponentMementoData>();

    near_plane             = memento_data.near_plane;
    far_plane              = memento_data.far_plane;
    fovy                   = memento_data.fovy;
    fovy_min                = memento_data.fovy_min;
    fovy_max                = memento_data.fovy_max;
    speed                 = memento_data.speed;
    viewport_size          = memento_data.viewport_size;
    fixed_up               = memento_data.fixed_up;
    up_epsilon             = memento_data.up_epsilon;
    cursor_move_offset_range = memento_data.cursor_move_offset_range;
    should_display_grid     = memento_data.should_display_grid;
}


#if 0

auto CameraComponent::MoveForward() -> self_type&
{
    return Move(GetForwardDir() * speed);
}

auto CameraComponent::MoveBackward() -> self_type&
{
    return Move(-GetForwardDir() * speed);
}

auto CameraComponent::MoveLeft() -> self_type&
{
    return Move(GetLeftDir() * speed);
}

auto CameraComponent::MoveRight() -> self_type&
{
    return Move(-GetLeftDir() * speed);
}

auto CameraComponent::MoveUp() -> self_type&
{
    return Move(GetUpDir() * speed);
}

auto CameraComponent::MoveDown() -> self_type&
{
    return Move(-GetUpDir() * speed);
}

auto CameraComponent::Move(const glm::vec3& Offset) -> self_type&
{
    position += Offset;
    Target += Offset;
    return *this;
}

auto CameraComponent::DragViewMove(const glm::vec2& Offset) -> self_type&
{
    Move(
        GetLeftDir() * Offset.x
        + GetUpDir() * Offset.y
    );
    return *this;
}

auto CameraComponent::RotateView(const glm::vec2& Offset) -> self_type&
{
    auto forward = GetForwardDir();

    //Horizontal
    auto x_angle_offset = -Offset.x;
    forward = glm::angleAxis(x_angle_offset, up) * forward;

    //Vertical
    auto y_angle_offset =
        glm::clamp(
            -Offset.y + _PitchAngle(),
            -glm::half_pi<float>() + up_epsilon,
            glm::half_pi<float>() - up_epsilon
        )
        - _PitchAngle();
    forward = glm::angleAxis(y_angle_offset, glm::cross(forward, up)) * forward;

    Target = position + forward;
    return *this;
}

auto CameraComponent::DragViewRotate(const glm::vec2& Offset) -> self_type&
{
    if(_TestCursorMove(Offset.x, Offset.y))
    {
        const auto& forward = GetForwardDir();

        //Horizontal
        auto x_angle_offset = -Offset.x;
        auto rotation = glm::angleAxis(x_angle_offset, up);
        position = rotation * position;
        Target = rotation * Target;

        //Vertical
        auto y_angle_offset =
            glm::clamp(
                -Offset.y + _PitchAngle(),
                -glm::half_pi<float>() + up_epsilon,
                glm::half_pi<float>() - up_epsilon
            )
            - _PitchAngle();
        rotation = glm::angleAxis(y_angle_offset, glm::cross(forward, up));
        position = rotation * position;
        Target = rotation * Target;
    }

    return *this;
}

auto CameraComponent::generate_view_mat4() const -> glm::mat4
{
    return glm::lookAt(position, Target, up);
}

auto CameraComponent::generate_proj_mat4() const -> glm::mat4
{
    return glm::perspective(
        Fov,
        viewport_size.x * 1.f / viewport_size.y,
        near_plane,
        far_plane
    );
}

auto CameraComponent::GenerateMat4(bool col_major) const -> glm::mat4
{
    if(col_major)
    {
        return generate_proj_mat4() * generate_view_mat4();
    }
    else
    {
        return glm::transpose(generate_view_mat4()) * glm::transpose(generate_proj_mat4());
    }
}

auto CameraComponent::GetForwardDir() const -> glm::vec3
{
    return glm::normalize(Target - position);
}

auto CameraComponent::GetLeftDir() const -> glm::vec3
{
    return glm::normalize(glm::cross(up, GetForwardDir()));
}

auto CameraComponent::GetUpDir() const -> glm::vec3
{
    return glm::normalize(glm::cross(GetForwardDir(), GetLeftDir()));
}

auto CameraComponent::_PitchAngle() const -> float
{
    const auto& forward = GetForwardDir();
    return glm::half_pi<float>() - glm::angle(forward, up);
}

auto CameraComponent::_YawAngle() const -> float
{
    ACDA_ASSERT(false && "This function is not working");

    const auto& forward = GetForwardDir();
    auto yaw_vec = forward - glm::dot(forward, vec3::pos_y());

    auto coef =
        glm::angle(yaw_vec, vec3::pos_x()) < glm::half_pi<float>() ?
        1 : -1;
    return coef * glm::angle(yaw_vec, vec3::neg_z());
}

auto CameraComponent::_RollAngle() const -> float
{
    ACDA_ASSERT(false && "This function is not working");

    const auto& forward = GetForwardDir();
    auto normal_of_forward_and_Up = glm::cross(forward, up);
    auto pos_uni_y_proj_on_forward_and_Up = up - glm::dot(up, normal_of_forward_and_Up);

    auto coef =
        glm::angle(up, normal_of_forward_and_Up) < glm::half_pi<float>() ?
        1 : -1;
    return coef * glm::angle(up, pos_uni_y_proj_on_forward_and_Up);
}

auto CameraComponent::_TestCursorMove(float x_offset, float y_offset) -> bool
{

    return is_in_range(x_offset, cursor_move_offset_range.x, cursor_move_offset_range.y)
        && is_in_range(y_offset, cursor_move_offset_range.x, cursor_move_offset_range.y);

}

#endif
