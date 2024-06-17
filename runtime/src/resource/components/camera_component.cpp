#include "pch.hpp"

#include "camera_component.hpp"

#include"core/math.hpp"

Arcadia::CameraComponent::CameraComponent(const nlohmann::json& json)
{
    NearPlane             = json.at("near_plane");
    FarPlane              = json.at("far_plane");
    Fovy                  = json.at("fovy");
    FovyMin               = json.at("fovy_min");
    FovyMax               = json.at("fovy_max");
    Speed                 = json.at("speed");
    ViewportSize          = Arcadia::IVec2::FromJson(json.at("viewport_size"));
    FixedUp               = json.at("fixed_up");
    UpEpsilon             = json.at("up_epsilon");
    CursorMoveOffsetRange = Arcadia::Vec2::FromJson(json.at("cursor_move_offset_range"));
}

auto Arcadia::CameraComponent::ToJson() const -> nlohmann::json
{
    return nlohmann::json{
        { "near_plane"              ,NearPlane },
        { "far_plane"               ,FarPlane },
        { "fovy"                    ,Fovy },
        { "fovy_min"                ,FovyMin },
        { "fovy_max"                ,FovyMax },
        { "speed"                   ,Speed},
        { "viewport_size"           ,Arcadia::IVec2::ToJson(ViewportSize) },
        { "fixed_up"                ,FixedUp },
        { "up_epsilon"              ,UpEpsilon },
        { "cursor_move_offset_range",Arcadia::Vec2::ToJson(CursorMoveOffsetRange) }
    };
}

auto Arcadia::CameraComponent::GenerateViewMat4(const glm::vec3& pos, const glm::vec3& dir) -> glm::mat4
{
    return glm::lookAt(pos, pos + dir, Up);
}

auto Arcadia::CameraComponent::GenerateProjMat4() const -> glm::mat4
{
    return glm::perspective(
        Fovy,
        ViewportSize.x * 1.f / ViewportSize.y,
        NearPlane,
        FarPlane
    );
}

auto Arcadia::CameraComponent::OnSnapshot() const -> std::shared_ptr<Arcadia::MementoDataBase>
{
    auto sp_memento = std::make_shared<Arcadia::CameraComponentMementoData>();

    sp_memento->NearPlane             = NearPlane;
    sp_memento->FarPlane              = FarPlane;
    sp_memento->Fovy                   = Fovy;
    sp_memento->FovyMin                = FovyMin;
    sp_memento->FovyMax                = FovyMax;
    sp_memento->Speed                 = Speed;
    sp_memento->ViewportSize          = ViewportSize;
    sp_memento->FixedUp               = FixedUp;
    sp_memento->UpEpsilon             = UpEpsilon;
    sp_memento->CursorMoveOffsetRange = CursorMoveOffsetRange;
    sp_memento->ShouldDisplayGrid     = ShouldDisplayGrid;

    return sp_memento;
}

void Arcadia::CameraComponent::OnRestore(const std::shared_ptr<Arcadia::MementoDataBase>& sp_memento_data)
{
    auto& memento_data = sp_memento_data->As<Arcadia::CameraComponentMementoData>();

    NearPlane             = memento_data.NearPlane;
    FarPlane              = memento_data.FarPlane;
    Fovy                   = memento_data.Fovy;
    FovyMin                = memento_data.FovyMin;
    FovyMax                = memento_data.FovyMax;
    Speed                 = memento_data.Speed;
    ViewportSize          = memento_data.ViewportSize;
    FixedUp               = memento_data.FixedUp;
    UpEpsilon             = memento_data.UpEpsilon;
    CursorMoveOffsetRange = memento_data.CursorMoveOffsetRange;
    ShouldDisplayGrid     = memento_data.ShouldDisplayGrid;
}


#if 0

auto Arcadia::CameraComponent::MoveForward() -> self_type&
{
    return Move(GetForwardDir() * Speed);
}

auto Arcadia::CameraComponent::MoveBackward() -> self_type&
{
    return Move(-GetForwardDir() * Speed);
}

auto Arcadia::CameraComponent::MoveLeft() -> self_type&
{
    return Move(GetLeftDir() * Speed);
}

auto Arcadia::CameraComponent::MoveRight() -> self_type&
{
    return Move(-GetLeftDir() * Speed);
}

auto Arcadia::CameraComponent::MoveUp() -> self_type&
{
    return Move(GetUpDir() * Speed);
}

auto Arcadia::CameraComponent::MoveDown() -> self_type&
{
    return Move(-GetUpDir() * Speed);
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
    return glm::half_pi<float>() - glm::angle(forward, Up);
}

auto Arcadia::CameraComponent::_YawAngle() const -> float
{
    ARCADIA_ASSERT(false && "This function is not working");

    const auto& forward = GetForwardDir();
    auto yaw_vec = forward - glm::dot(forward, Arcadia::Vec3::PosY());

    auto coef =
        glm::angle(yaw_vec, Arcadia::Vec3::PosX()) < glm::half_pi<float>() ?
        1 : -1;
    return coef * glm::angle(yaw_vec, Arcadia::Vec3::NegZ());
}

auto Arcadia::CameraComponent::_RollAngle() const -> float
{
    ARCADIA_ASSERT(false && "This function is not working");

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

#endif
