#include "pch.hpp"
#include "transform_component.hpp"

#include"core/math.hpp"
#include"core/assert.hpp"

Arcadia::TransformComponent::TransformComponent(const nlohmann::json& json):
    _Flags(json.at("flags")),
    _Position(GlmVec3::FromJson(json.at("position"))),
    _RotationEularAngle(GlmVec3::FromJson(json.at("rotation"))),
    _Direction(GlmVec3::FromJson(json.at("direction"))),
    _Scale(GlmVec3::FromJson(json.at("scale"))),
    _Pivot(GlmVec3::FromJson(json.at("pivot")))
{}

auto Arcadia::TransformComponent::ToJson() const -> nlohmann::json
{
    nlohmann::json json{
        {"flags", _Flags},
        {"position",GlmVec3::ToJson(_Position)},
        {"rotation",GlmVec3::ToJson(_RotationEularAngle)},
        {"direction",GlmVec3::ToJson(_Direction)},
        {"scale"   ,GlmVec3::ToJson(_Scale)},
        {"pivot"   ,GlmVec3::ToJson(_Pivot)}
    };

    return json;
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
        _TransformMatrix = GlmMat4::CreateIdentity();
        _TransformMatrix = glm::translate(_TransformMatrix, _Position);
        _TransformMatrix = _TransformMatrix * glm::mat4_cast(_RotationQuaternion);
        _TransformMatrix = glm::scale(_TransformMatrix, _Scale);

        _TransformMatrixDirty = false;
    }
    return _TransformMatrix;
}

