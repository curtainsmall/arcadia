#include "camera_component.hpp"

#include "core/command.hpp"
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
    _NearPlane = json.at("near_plane");
    _FarPlane = json.at("far_plane");
    _FovY = json.at("fovy");
    _FovYMin = json.at("fovy_min");
    _FovYMax = json.at("fovy_max");
    _Speed = json.at("speed");
    _ViewportSize = Glm::Int32Vec2_FromJson(json.at("viewport_size"));
    _UpAxisFixed = json.at("fixed_up");
    _UpAxisAngleEpsilon = json.at("up_epsilon");
    _CursorMoveOffsetRange = Glm::Vec2_FromJson(json.at("cursor_move_offset_range"));
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
        { "viewport_size"           ,Glm::Int32Vec2_ToJson(_ViewportSize) },
        { "fixed_up"                ,_UpAxisFixed },
        { "up_epsilon"              ,_UpAxisAngleEpsilon },
        { "cursor_move_offset_range",Glm::Vec2_ToJson(_CursorMoveOffsetRange) }
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

auto Arcadia::CameraComponent::OnSnapshot() const -> std::unique_ptr<MementoType>
{
    std::unique_ptr<MementoType> memento_uptr = std::make_unique<MementoType>();
    memento_uptr->NearPlane = GetNearPlane();
    memento_uptr->FarPlane = GetFarPlane();
    memento_uptr->FovY = GetFovY();
    memento_uptr->FovYMin = GetFovYMin();
    memento_uptr->FovYMax = GetFovYMax();
    memento_uptr->Speed = GetSpeed();
    memento_uptr->ViewportSize = GetViewportSize();
    memento_uptr->UpAxisFixed = IsUpAxisFixed();
    memento_uptr->UpAxisAngleEpsilon = GetUpAxisAngleEpsilon();
    memento_uptr->CursorMoveOffsetRange = GetCursorMoveOffsetRange();
    memento_uptr->GridDisplaying = IsGridDisplaying();
    return memento_uptr;
}

void Arcadia::CameraComponent::OnRestore(const std::unique_ptr<MementoType>& memento_uptr)
{
    SetNearPlane(memento_uptr->NearPlane);
    SetFarPlane(memento_uptr->FarPlane);
    SetFovY(memento_uptr->FovY);
    SetFovYMin(memento_uptr->FovYMin);
    SetFovYMax(memento_uptr->FovYMax);
    SetSpeed(memento_uptr->Speed);
    SetViewportSize(memento_uptr->ViewportSize);
    SetUpAxisFixed(memento_uptr->UpAxisFixed);
    SetUpAxisAngleEpsilon(memento_uptr->UpAxisAngleEpsilon);
    SetCursorMoveOffsetRange(memento_uptr->CursorMoveOffsetRange);
    SetGridDisplaying(memento_uptr->GridDisplaying);
}