#include "pch.hpp"

#include "camera_component.hpp"

#include "core/command/command.hpp"
#include "core/math.hpp"

auto Arcadia::CameraComponent::GetUpAxis() -> const glm::vec3&
{
    return _UpAxis;
}

void Arcadia::CameraComponent::SetUpAxis(const glm::vec3& up_axis)
{
    _UpAxis = up_axis;
}

Arcadia::CameraComponent::CameraComponent(const nlohmann::json& json)
{
    _NearPlane             = json.at("near_plane");
    _FarPlane              = json.at("far_plane");
    _FovY                  = json.at("fovy");
    _FovYMin               = json.at("fovy_min");
    _FovYMax               = json.at("fovy_max");
    _Speed                 = json.at("speed");
    _ViewportSize          = GlmInt32Vec2::FromJson(json.at("viewport_size"));
    _UpAxisFixed               = json.at("fixed_up");
    _UpAxisAngleEpsilon             = json.at("up_epsilon");
    _CursorMoveOffsetRange = GlmVec2::FromJson(json.at("cursor_move_offset_range"));
}

auto Arcadia::CameraComponent::ToJson() const -> nlohmann::json
{
    return nlohmann::json{
        { "near_plane"              ,_NearPlane },
        { "far_plane"               ,_FarPlane },
        { "fovy"                    ,_FovY },
        { "fovy_min"                ,_FovYMin },
        { "fovy_max"                ,_FovYMax },
        { "speed"                   ,_Speed},
        { "viewport_size"           ,GlmInt32Vec2::ToJson(_ViewportSize) },
        { "fixed_up"                ,_UpAxisFixed },
        { "up_epsilon"              ,_UpAxisAngleEpsilon },
        { "cursor_move_offset_range",GlmVec2::ToJson(_CursorMoveOffsetRange) }
    };
}

auto Arcadia::CameraComponent::GetNearPlane() const -> float
{
    return _NearPlane;
}

void Arcadia::CameraComponent::SetNearPlane(float near_plane)
{
    _NearPlane = near_plane;
}

auto Arcadia::CameraComponent::GetFarPlane() const -> float
{
    return _FarPlane;
}

void Arcadia::CameraComponent::SetFarPlane(float far_plane)
{
    _FarPlane = far_plane;
}

auto Arcadia::CameraComponent::GetFovY() const -> float
{
    return _FovY;
}

void Arcadia::CameraComponent::SetFovY(float fov_y)
{
    _FovY = fov_y;
}

auto Arcadia::CameraComponent::GetFovYMin() const -> float
{
    return _FovYMin;
}

void Arcadia::CameraComponent::SetFovYMin(float fov_y_min)
{
    _FovYMin = fov_y_min;
}

auto Arcadia::CameraComponent::GetFovYMax() const -> float
{
    return _FovYMax;
}

void Arcadia::CameraComponent::SetFovYMax(float fov_y_max)
{
    _FovYMax = fov_y_max;
}

auto Arcadia::CameraComponent::GetSpeed() const -> float
{
    return _Speed;
}

void Arcadia::CameraComponent::SetSpeed(float speed)
{
    _Speed = speed;
}

auto Arcadia::CameraComponent::GetViewportSize() const -> const glm::i32vec2&
{
    return _ViewportSize;
}

void Arcadia::CameraComponent::SetViewportSize(const glm::i32vec2& viewport_size)
{
    _ViewportSize = viewport_size;
}

auto Arcadia::CameraComponent::IsUpAxisFixed() const -> bool
{
    return _UpAxisFixed;
}

void Arcadia::CameraComponent::SetUpAxisFixed(bool fixed)
{
    _UpAxisFixed = fixed;
}

auto Arcadia::CameraComponent::GetUpAxisAngleEpsilon() const -> float
{
    return _UpAxisAngleEpsilon;
}

void Arcadia::CameraComponent::SetUpAxisAngleEpsilon(float epsilon)
{
    _UpAxisAngleEpsilon = epsilon;
}

auto Arcadia::CameraComponent::GetCursorMoveOffsetRange() const -> const glm::vec2&
{
    return _CursorMoveOffsetRange;
}

void Arcadia::CameraComponent::SetCursorMoveOffsetRange(const glm::vec2& range)
{
    _CursorMoveOffsetRange = range;
}

auto Arcadia::CameraComponent::IsGridDisplaying() const -> bool
{
    return _GridDisplaying;
}

void Arcadia::CameraComponent::SetGridDisplaying(bool displaying)
{
    _GridDisplaying = displaying;
}

auto Arcadia::CameraComponent::GenerateViewMat4(const glm::vec3& pos, const glm::vec3& dir) -> glm::mat4
{
    return glm::lookAt(pos, pos + dir, _UpAxis);
}

auto Arcadia::CameraComponent::GenerateProjectiveMat4() const -> glm::mat4
{
    return glm::perspective(
        _FovY,
        _ViewportSize.x * 1.f / _ViewportSize.y,
        _NearPlane,
        _FarPlane
    );
}

#if 0

auto Arcadia::CameraComponent::MoveForward() -> SelfType&
{
    return Move(GetForwardDir() * _Speed);
}

auto Arcadia::CameraComponent::MoveBackward() -> SelfType&
{
    return Move(-GetForwardDir() * _Speed);
}

auto Arcadia::CameraComponent::MoveLeft() -> SelfType&
{
    return Move(GetLeftDir() * _Speed);
}

auto Arcadia::CameraComponent::MoveRight() -> SelfType&
{
    return Move(-GetLeftDir() * _Speed);
}

auto Arcadia::CameraComponent::MoveUp() -> SelfType&
{
    return Move(GetUpDir() * _Speed);
}

auto Arcadia::CameraComponent::MoveDown() -> SelfType&
{
    return Move(-GetUpDir() * _Speed);
}

auto Arcadia::CameraComponent::Move(const glm::vec3& Offset) -> SelfType&
{
    Position += Offset;
    Target += Offset;
    return *this;
}

auto Arcadia::CameraComponent::DragViewMove(const glm::vec2& Offset) -> SelfType&
{
    Move(
        GetLeftDir() * Offset.x
        + GetUpDir() * Offset.y
    );
    return *this;
}

auto Arcadia::CameraComponent::RotateView(const glm::vec2& Offset) -> SelfType&
{
    auto forward = GetForwardDir();

    //Horizontal
    auto x_angle_offset = -Offset.x;
    forward = glm::angleAxis(x_angle_offset, _UpAxis) * forward;

    //Vertical
    auto y_angle_offset =
        glm::clamp(
            -Offset.y + _PitchAngle(),
            -glm::half_pi<float>() + _UpAxisAngleEpsilon,
            glm::half_pi<float>() - _UpAxisAngleEpsilon
        )
        - _PitchAngle();
    forward = glm::angleAxis(y_angle_offset, glm::cross(forward, _UpAxis)) * forward;

    Target = Position + forward;
    return *this;
}

auto Arcadia::CameraComponent::DragViewRotate(const glm::vec2& Offset) -> SelfType&
{
    if(_TestCursorMove(Offset.x, Offset.y))
    {
        const auto& forward = GetForwardDir();

        //Horizontal
        auto x_angle_offset = -Offset.x;
        auto Rotation = glm::angleAxis(x_angle_offset, _UpAxis);
        Position = Rotation * Position;
        Target = Rotation * Target;

        //Vertical
        auto y_angle_offset =
            glm::clamp(
                -Offset.y + _PitchAngle(),
                -glm::half_pi<float>() + _UpAxisAngleEpsilon,
                glm::half_pi<float>() - _UpAxisAngleEpsilon
            )
            - _PitchAngle();
        Rotation = glm::angleAxis(y_angle_offset, glm::cross(forward, _UpAxis));
        Position = Rotation * Position;
        Target = Rotation * Target;
    }

    return *this;
}

auto Arcadia::CameraComponent::GenerateViewMat4() const -> glm::mat4
{
    return glm::lookAt(Position, Target, _UpAxis);
}

auto Arcadia::CameraComponent::GenerateProjectiveMat4() const -> glm::mat4
{
    return glm::perspective(
        Fov,
        _ViewportSize.x * 1.f / _ViewportSize.y,
        _NearPlane,
        _FarPlane
    );
}

auto Arcadia::CameraComponent::GenerateMat4(bool col_major) const -> glm::mat4
{
    if(col_major)
    {
        return GenerateProjectiveMat4() * GenerateViewMat4();
    }
    else
    {
        return glm::transpose(GenerateViewMat4()) * glm::transpose(GenerateProjectiveMat4());
    }
}

auto Arcadia::CameraComponent::GetForwardDir() const -> glm::vec3
{
    return glm::normalize(Target - Position);
}

auto Arcadia::CameraComponent::GetLeftDir() const -> glm::vec3
{
    return glm::normalize(glm::cross(_UpAxis, GetForwardDir()));
}

auto Arcadia::CameraComponent::GetUpDir() const -> glm::vec3
{
    return glm::normalize(glm::cross(GetForwardDir(), GetLeftDir()));
}

auto Arcadia::CameraComponent::_PitchAngle() const -> float
{
    const auto& forward = GetForwardDir();
    return glm::half_pi<float>() - glm::angle(forward, _UpAxis);
}

auto Arcadia::CameraComponent::_YawAngle() const -> float
{
    ACDA_UNREACHABLE("This function is not working");

    const auto& forward = GetForwardDir();
    auto yaw_vec = forward - glm::dot(forward, vec3::CreateUnitPositiveY());

    auto coef =
        glm::angle(yaw_vec, vec3::CreateUnitPositiveX()) < glm::half_pi<float>() ?
        1 : -1;
    return coef * glm::angle(yaw_vec, vec3::CreateUnitNegativeZ());
}

auto Arcadia::CameraComponent::_RollAngle() const -> float
{
    ACDA_UNREACHABLE("This function is not working");

    const auto& forward = GetForwardDir();
    auto normal_of_forward_and_Up = glm::cross(forward, _UpAxis);
    auto pos_uni_y_proj_on_forward_and_Up = _UpAxis - glm::dot(_UpAxis, normal_of_forward_and_Up);

    auto coef =
        glm::angle(_UpAxis, normal_of_forward_and_Up) < glm::half_pi<float>() ?
        1 : -1;
    return coef * glm::angle(_UpAxis, pos_uni_y_proj_on_forward_and_Up);
}

auto Arcadia::CameraComponent::_TestCursorMove(float x_offset, float y_offset) -> bool
{
    return IsInRange(x_offset, _CursorMoveOffsetRange.x, _CursorMoveOffsetRange.y)
        && IsInRange(y_offset, _CursorMoveOffsetRange.x, _CursorMoveOffsetRange.y);
}

#endif