#include "player_controller.hpp"

auto Arcadia::PlayerController::IsActive() const -> bool
{
    return _Active;
}

void Arcadia::PlayerController::SetActive(bool active)
{
    _Active = active;
}

auto Arcadia::PlayerController::GetActiveEntityId() const -> EntityId
{
    return _ActiveEntityId;
}

void Arcadia::PlayerController::SetActiveEntityId(EntityId entity_id)
{
    _ActiveEntityId = entity_id;
}