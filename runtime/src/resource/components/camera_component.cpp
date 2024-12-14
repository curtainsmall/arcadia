#include "pch.hpp"

#include "camera_component.hpp"

#include"core/math.hpp"

Arcadia::CameraComponent::CameraComponent(const nlohmann::json& json)
{
    NearPlane             = json.at("near_plane");
    FarPlane              = json.at("far_plane");
    FovY                  = json.at("fovy");
    FovYMin               = json.at("fovy_min");
    FovYMax               = json.at("fovy_max");
    Speed                 = json.at("speed");
    ViewportSize          = IntVec2::FromJson(json.at("viewport_size"));
    FixedUp               = json.at("fixed_up");
    UpEpsilon             = json.at("up_epsilon");
    CursorMoveOffsetRange = GlmVec2::FromJson(json.at("cursor_move_offset_range"));
}

auto Arcadia::CameraComponent::ToJson() const -> nlohmann::json
{
    return nlohmann::json{
        { "near_plane"              ,NearPlane },
        { "far_plane"               ,FarPlane },
        { "fovy"                    ,FovY },
        { "fovy_min"                ,FovYMin },
        { "fovy_max"                ,FovYMax },
        { "speed"                   ,Speed},
        { "viewport_size"           ,IntVec2::ToJson(ViewportSize) },
        { "fixed_up"                ,FixedUp },
        { "up_epsilon"              ,UpEpsilon },
        { "cursor_move_offset_range",GlmVec2::ToJson(CursorMoveOffsetRange) }
    };
}

auto Arcadia::CameraComponent::GenerateViewMat4(const glm::vec3& pos, const glm::vec3& dir) -> glm::mat4
{
    return glm::lookAt(pos, pos + dir, Up);
}

auto Arcadia::CameraComponent::GenerateProjectiveMat4() const -> glm::mat4
{
    return glm::perspective(
        FovY,
        ViewportSize.x * 1.f / ViewportSize.y,
        NearPlane,
        FarPlane
    );
}

auto Arcadia::CameraComponent::OnSnapshot() const -> std::shared_ptr<MementoDataBase>
{
    auto sp_memento = std::make_shared<CameraComponentMementoData>();

    sp_memento->NearPlane             = NearPlane;
    sp_memento->FarPlane              = FarPlane;
    sp_memento->FovY                   = FovY;
    sp_memento->FovYMin                = FovYMin;
    sp_memento->FovYMax                = FovYMax;
    sp_memento->Speed                 = Speed;
    sp_memento->ViewportSize          = ViewportSize;
    sp_memento->FixedUp               = FixedUp;
    sp_memento->UpEpsilon             = UpEpsilon;
    sp_memento->CursorMoveOffsetRange = CursorMoveOffsetRange;
    sp_memento->ShouldDisplayGrid     = ShouldDisplayGrid;

    return sp_memento;
}

void Arcadia::CameraComponent::OnRestore(const std::shared_ptr<MementoDataBase>& sp_memento_data)
{
    auto& memento_data = sp_memento_data->CastTo<CameraComponentMementoData>();

    NearPlane             = memento_data.NearPlane;
    FarPlane              = memento_data.FarPlane;
    FovY                   = memento_data.FovY;
    FovYMin                = memento_data.FovYMin;
    FovYMax                = memento_data.FovYMax;
    Speed                 = memento_data.Speed;
    ViewportSize          = memento_data.ViewportSize;
    FixedUp               = memento_data.FixedUp;
    UpEpsilon             = memento_data.UpEpsilon;
    CursorMoveOffsetRange = memento_data.CursorMoveOffsetRange;
    ShouldDisplayGrid     = memento_data.ShouldDisplayGrid;
}

#if 0

auto Arcadia::CameraComponent::MoveForward() -> SelfType&
{
    return Move(GetForwardDir() * Speed);
}

auto Arcadia::CameraComponent::MoveBackward() -> SelfType&
{
    return Move(-GetForwardDir() * Speed);
}

auto Arcadia::CameraComponent::MoveLeft() -> SelfType&
{
    return Move(GetLeftDir() * Speed);
}

auto Arcadia::CameraComponent::MoveRight() -> SelfType&
{
    return Move(-GetLeftDir() * Speed);
}

auto Arcadia::CameraComponent::MoveUp() -> SelfType&
{
    return Move(GetUpDir() * Speed);
}

auto Arcadia::CameraComponent::MoveDown() -> SelfType&
{
    return Move(-GetUpDir() * Speed);
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
    forward = glm::angleAxis(x_angle_offset, Up) * forward;

    //Vertical
    auto y_angle_offset =
        glm::clamp(
            -Offset.y + _PitchAngle(),
            -glm::half_pi<float>() + UpEpsilon,
            glm::half_pi<float>() - UpEpsilon
        )
        - _PitchAngle();
    forward = glm::angleAxis(y_angle_offset, glm::cross(forward, Up)) * forward;

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
        auto Rotation = glm::angleAxis(x_angle_offset, Up);
        Position = Rotation * Position;
        Target = Rotation * Target;

        //Vertical
        auto y_angle_offset =
            glm::clamp(
                -Offset.y + _PitchAngle(),
                -glm::half_pi<float>() + UpEpsilon,
                glm::half_pi<float>() - UpEpsilon
            )
            - _PitchAngle();
        Rotation = glm::angleAxis(y_angle_offset, glm::cross(forward, Up));
        Position = Rotation * Position;
        Target = Rotation * Target;
    }

    return *this;
}

auto Arcadia::CameraComponent::GenerateViewMat4() const -> glm::mat4
{
    return glm::lookAt(Position, Target, Up);
}

auto Arcadia::CameraComponent::GenerateProjectiveMat4() const -> glm::mat4
{
    return glm::perspective(
        Fov,
        ViewportSize.x * 1.f / ViewportSize.y,
        NearPlane,
        FarPlane
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
    return glm::normalize(glm::cross(Up, GetForwardDir()));
}

auto Arcadia::CameraComponent::GetUpDir() const -> glm::vec3
{
    return glm::normalize(glm::cross(GetForwardDir(), GetLeftDir()));
}

auto Arcadia::CameraComponent::_PitchAngle() const -> float
{
    const auto& forward = GetForwardDir();
    return glm::half_pi<float>() - glm::angle(forward, Up);
}

auto Arcadia::CameraComponent::_YawAngle() const -> float
{
    ACDA_ASSERT(false && "This function is not working");

    const auto& forward = GetForwardDir();
    auto yaw_vec = forward - glm::dot(forward, vec3::CreateUnitPositiveY());

    auto coef =
        glm::angle(yaw_vec, vec3::CreateUnitPositiveX()) < glm::half_pi<float>() ?
        1 : -1;
    return coef * glm::angle(yaw_vec, vec3::CreateUnitNegativeZ());
}

auto Arcadia::CameraComponent::_RollAngle() const -> float
{
    ACDA_ASSERT(false && "This function is not working");

    const auto& forward = GetForwardDir();
    auto normal_of_forward_and_Up = glm::cross(forward, Up);
    auto pos_uni_y_proj_on_forward_and_Up = Up - glm::dot(Up, normal_of_forward_and_Up);

    auto coef =
        glm::angle(Up, normal_of_forward_and_Up) < glm::half_pi<float>() ?
        1 : -1;
    return coef * glm::angle(Up, pos_uni_y_proj_on_forward_and_Up);
}

auto Arcadia::CameraComponent::_TestCursorMove(float x_offset, float y_offset) -> bool
{
    return IsInRange(x_offset, CursorMoveOffsetRange.x, CursorMoveOffsetRange.y)
        && IsInRange(y_offset, CursorMoveOffsetRange.x, CursorMoveOffsetRange.y);
}

#endif