#include "pch.hpp"
#include "entity_id.hpp"

Arcadia::EntityId::EntityId(entt::entity id):
    _Id(id)
{}

auto Arcadia::EntityId::operator=(entt::entity id)
{
    _Id = id;
}

auto Arcadia::EntityId::operator==(const EntityId& rhs) const -> bool
{
    return _Id == rhs._Id;
}

Arcadia::EntityId::operator bool() const
{
    return !IsNull();
}

Arcadia::EntityId::operator entt::entity() const
{
    return GetValue();
}

auto Arcadia::EntityId::GetValue() const -> entt::entity
{
    return _Id;
}

void Arcadia::EntityId::SetNull()
{
    _Id = entt::null;
}

auto Arcadia::EntityId::IsNull() const -> bool
{
    return _Id == entt::null;
}
