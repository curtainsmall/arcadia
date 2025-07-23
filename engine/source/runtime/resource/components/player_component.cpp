#include "resource/components/player_component.hpp"

Arcadia::PlayerComponent::PlayerComponent(const nlohmann::json& json)
{
}

auto Arcadia::PlayerComponent::ToJson() const -> nlohmann::json
{
    return nlohmann::json();
}