
#include "transform_component.hpp"

#include "core/assert.hpp"
#include "core/math.hpp"

Arcadia::TransformComponent::TransformComponent(const nlohmann::json& json):
    _Flags(json.at("flags")),
    _Position(Glm::Vec3_FromJson(json.at("position"))),
    _RotationEularAngle(Glm::Vec3_FromJson(json.at("rotation"))),
    _Direction(Glm::Vec3_FromJson(json.at("direction"))),
    _Scale(Glm::Vec3_FromJson(json.at("scale"))),
    _Pivot(Glm::Vec3_FromJson(json.at("pivot")))
{
}

auto Arcadia::TransformComponent::ToJson() const -> nlohmann::json
{
    nlohmann::json json{
        {"flags", _Flags},
        {"position",Glm::Vec3_ToJson(_Position)},
        {"rotation",Glm::Vec3_ToJson(_RotationEularAngle)},
        {"direction",Glm::Vec3_ToJson(_Direction)},
        {"scale"   ,Glm::Vec3_ToJson(_Scale)},
        {"pivot"   ,Glm::Vec3_ToJson(_Pivot)}
    };

    return json;
}

auto Arcadia::TransformComponent::OnSnapshot() const -> std::unique_ptr<MementoType>
{
    std::unique_ptr<MementoType> memento_uptr = std::make_unique<MementoType>();
    memento_uptr->Flags = GetFlags();
    memento_uptr->Position = GetPosition();
    memento_uptr->RotationQuaternion = GetRotationQuaternion();
    memento_uptr->Scale = GetScale();
    memento_uptr->Direction = GetDirection();
    memento_uptr->Pivot = GetPivot();
    return memento_uptr;
}

void Arcadia::TransformComponent::OnRestore(const std::unique_ptr<MementoType>& memento_uptr)
{
    SetFlags(memento_uptr->Flags);
    SetPosition(memento_uptr->Position);
    SetRotationQuaternion(memento_uptr->RotationQuaternion);
    SetScale(memento_uptr->Scale);
    SetDirection(memento_uptr->Direction);
    SetPivot(memento_uptr->Pivot);
}

auto Arcadia::TransformComponent::GetFlags() const -> TransformComponentFlags
{
    return _Flags;
}

void Arcadia::TransformComponent::SetFlags(TransformComponentFlags flags)
{
    _Flags = flags;
}

auto Arcadia::TransformComponent::CheckFlag(TransformComponentFlags flag) const -> bool
{
    return !!(_Flags & flag);
}

void Arcadia::TransformComponent::AddFlag(TransformComponentFlags flag)
{
    _Flags |= flag;
}

void Arcadia::TransformComponent::RemoveFlag(TransformComponentFlags flag)
{
    _Flags &= ~flag;
}

auto Arcadia::TransformComponent::GetPosition() const -> const glm::vec3&
{
    return _Position;
}

void Arcadia::TransformComponent::SetPosition(const glm::vec3& vec)
{
    _Position = vec;
    _TransformMatrixDirty = true;
}

void Arcadia::TransformComponent::IncreasePosition(const glm::vec3& vec)
{
    _Position += vec;
    _TransformMatrixDirty = true;
}

auto Arcadia::TransformComponent::GetRotationEularAngle() const -> const glm::vec3&
{
    return _RotationEularAngle;
}

auto Arcadia::TransformComponent::GetRotationQuaternion() const -> const glm::quat&
{
    return _RotationQuaternion;
}

void Arcadia::TransformComponent::SetRotationEularAngle(const glm::vec3& vec)
{
    _RotationEularAngle = vec;
    _RotationQuaternion = _RotationEularAngle;
    _TransformMatrixDirty = true;
}

void Arcadia::TransformComponent::IncreaseRotationEularAngle(const glm::vec3& vec)
{
    _RotationEularAngle += vec;
    _RotationQuaternion = _RotationEularAngle;
    _TransformMatrixDirty = true;
}

void Arcadia::TransformComponent::SetRotationQuaternion(const glm::quat& quat)
{
    _RotationQuaternion = quat;
    _RotationEularAngle = glm::eulerAngles(_RotationQuaternion);
    _TransformMatrixDirty = true;
}

void Arcadia::TransformComponent::IncreaseRotationQuaternion(const glm::quat& quat)
{
    _RotationQuaternion += quat;
    _RotationEularAngle = glm::eulerAngles(_RotationQuaternion);
    _TransformMatrixDirty = true;
}

auto Arcadia::TransformComponent::GetDirection() const -> const glm::vec3&
{
    return _Direction;
}

void Arcadia::TransformComponent::SetDirection(const glm::vec3& vec)
{
    _Direction = vec;
    _TransformMatrixDirty = true;
}

void Arcadia::TransformComponent::IncreaseDirection(const glm::vec3& vec)
{
    _Direction += vec;
    _TransformMatrixDirty = true;
}

auto Arcadia::TransformComponent::GetScale() const -> const glm::vec3&
{
    return _Scale;
}

void Arcadia::TransformComponent::SetScale(const glm::vec3& vec)
{
    _Scale = vec;
    _TransformMatrixDirty = true;
}

void Arcadia::TransformComponent::IncreaseScale(const glm::vec3& vec)
{
    _Scale += vec;
    _TransformMatrixDirty = true;
}

auto Arcadia::TransformComponent::GetPivot() const -> const glm::vec3&
{
    return _Pivot;
}

void Arcadia::TransformComponent::SetPivot(const glm::vec3& vec)
{
    _Pivot = vec;
    _TransformMatrixDirty = true;
}

void Arcadia::TransformComponent::IncreasePivot(const glm::vec3& vec)
{
    _Pivot += vec;
    _TransformMatrixDirty = true;
}

auto Arcadia::TransformComponent::GetTransformMatrix() const -> const glm::mat4&
{
    if(_TransformMatrixDirty)
    {
        _TransformMatrix = Glm::Mat4_CreateIdentity();
        _TransformMatrix = glm::translate(_TransformMatrix, _Position);
        _TransformMatrix = _TransformMatrix * glm::mat4_cast(_RotationQuaternion);
        _TransformMatrix = glm::scale(_TransformMatrix, _Scale);

        _TransformMatrixDirty = false;
    }
    return _TransformMatrix;
}