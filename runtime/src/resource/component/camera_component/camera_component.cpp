#include "pch.hpp"
#include "camera_component.hpp"

#include"core/math.hpp"

Arcadia::CameraComponent::CameraComponent(const nlohmann::json& json)
{
    Position              = Arcadia::Vec3::FromJson(json.at("Position"));
    Target                = Arcadia::Vec3::FromJson(json.at("Target"));
    Up                    = Arcadia::Vec3::FromJson(json.at("Up"));
    NearPlane             = json.at("near_plane");
    FarPlane              = json.at("far_plane");
    Fov                   = json.at("fov");
    FovMin                = json.at("fov_min");
    FovMax                = json.at("fov_max");
    Speed                 = json.at("Speed");
    ViewportSize          = Arcadia::IVec2::FromJson(json.at("viewport_size"));
    FixedUp               = json.at("fixed_Up");
    UpEpsilon             = json.at("UpEpsilon");
    CursorMoveOffsetRange = Arcadia::Vec2::FromJson(json.at("cursor_move_offset_range"));
}

auto Arcadia::CameraComponent::ToJson() const -> nlohmann::json
{
    return nlohmann::json{
        { "Position"                ,Arcadia::Vec3::ToJson(Position) },
        { "Target"                  ,Arcadia::Vec3::ToJson(Target) },
        { "Up"                      ,Arcadia::Vec3::ToJson(Up) },
        { "near_plane"              ,NearPlane },
        { "far_plane"               ,FarPlane },
        { "fov"                     ,Fov },
        { "fov_min"                 ,FovMin },
        { "fov_max"                 ,FovMax },
        { "Speed"                   ,Speed},
        { "viewport_size"           ,Arcadia::IVec2::ToJson(ViewportSize) },
        { "fixed_Up"                ,FixedUp },
        { "UpEpsilon"              ,UpEpsilon },
        { "cursor_move_offset_range",Arcadia::Vec2::ToJson(CursorMoveOffsetRange) }
    };
}

auto Arcadia::CameraComponent::OnSnapshot() const -> memento_data_type
{
    memento_data_type memento{};

    memento.Position              = Position;
    memento.Target                = Target;
    memento.Up                    = Up;
    memento.NearPlane             = NearPlane;
    memento.FarPlane              = FarPlane;
    memento.Fov                   = Fov;
    memento.FovMin                = FovMin;
    memento.FovMax                = FovMax;
    memento.Speed                 = Speed;
    memento.ViewportSize          = ViewportSize;
    memento.FixedUp               = FixedUp;
    memento.UpEpsilon             = UpEpsilon;
    memento.CursorMoveOffsetRange = CursorMoveOffsetRange;
    memento.ShouldDisplayGrid     = ShouldDisplayGrid;

    return memento;
}

void Arcadia::CameraComponent::OnRestore(const memento_data_type& memento)
{
    Position              = memento.Position;
    Target                = memento.Target;
    Up                    = memento.Up;
    NearPlane             = memento.NearPlane;
    FarPlane              = memento.FarPlane;
    Fov                   = memento.Fov;
    FovMin                = memento.FovMin;
    FovMax                = memento.FovMax;
    Speed                 = memento.Speed;
    ViewportSize          = memento.ViewportSize;
    FixedUp               = memento.FixedUp;
    UpEpsilon             = memento.UpEpsilon;
    CursorMoveOffsetRange = memento.CursorMoveOffsetRange;
    ShouldDisplayGrid     = memento.ShouldDisplayGrid;
}




auto Arcadia::CameraComponent::MoveForward() -> self_type&
{
    const auto movement = GetForwardDir() * Speed;
    Position += movement;
    Target += movement;
    return *this;
}

auto Arcadia::CameraComponent::MoveBackward() -> self_type&
{
    const auto movement = GetForwardDir() * Speed;
    Position -= movement;
    Target -= movement;
    return *this;
}

auto Arcadia::CameraComponent::MoveLeft() -> self_type&
{
    const auto movement = glm::cross(GetForwardDir(), Up) * Speed;
    Position -= movement;
    Target -= movement;
    return *this;
}

auto Arcadia::CameraComponent::MoveRight() -> self_type&
{
    const auto movement = glm::cross(GetForwardDir(), Up) * Speed;
    Position += movement;
    Target += movement;
    return *this;
}

auto Arcadia::CameraComponent::Move(const glm::vec3& Offset) -> self_type&
{
    Position += Offset;
    Target += Offset;
    return *this;
}

auto Arcadia::CameraComponent::DragViewMove(const glm::vec2& Offset) -> self_type&
{
    Move(
        GetLeftDir() * Offset.x
        + GetUpDir() * Offset.y
    );
    return *this;
}

auto Arcadia::CameraComponent::RotateView(const glm::vec2& Offset) -> self_type&
{
    if(_TestCursorMove(Offset.x, Offset.y))
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
    }
    return *this;
}

auto Arcadia::CameraComponent::DragViewRotate(const glm::vec2& Offset) -> self_type&
{
    if(_TestCursorMove(Offset.x, Offset.y))
    {
        const auto& forward = GetForwardDir();

        //Horizontal
        auto x_angle_offset = -Offset.x;
        auto rotation = glm::angleAxis(x_angle_offset, Up);
        Position = rotation * Position;
        Target = rotation * Target;

        //Vertical
        auto y_angle_offset =
            glm::clamp(
                -Offset.y + _PitchAngle(),
                -glm::half_pi<float>() + UpEpsilon,
                glm::half_pi<float>() - UpEpsilon
            )
            - _PitchAngle();
        rotation = glm::angleAxis(y_angle_offset, glm::cross(forward, Up));
        Position = rotation * Position;
        Target = rotation * Target;
    }

    return *this;
}

auto Arcadia::CameraComponent::GenerateViewMat4() const -> glm::mat4
{
    return glm::lookAt(Position, Target, Up);
}

auto Arcadia::CameraComponent::GenerateProjMat4() const -> glm::mat4
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
        return GenerateProjMat4() * GenerateViewMat4();
    }
    else
    {
        return glm::transpose(GenerateViewMat4()) * glm::transpose(GenerateProjMat4());
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
    return glm::angle(forward, Up) - glm::half_pi<float>();
}

auto Arcadia::CameraComponent::_YawAngle() const -> float
{
    const auto& forward = GetForwardDir();
    auto yaw_vec = forward - glm::dot(forward, Arcadia::Vec3::PosY());

    auto coef =
        glm::angle(yaw_vec, Arcadia::Vec3::PosX()) < glm::half_pi<float>() ?
        1 : -1;
    return coef * glm::angle(yaw_vec, Arcadia::Vec3::NegZ());
}

auto Arcadia::CameraComponent::_RollAngle() const -> float
{
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

    return Arcadia::IsInRange(x_offset, CursorMoveOffsetRange.x, CursorMoveOffsetRange.y)
        && Arcadia::IsInRange(y_offset, CursorMoveOffsetRange.x, CursorMoveOffsetRange.y);

}
