#include"pch.hpp"

#include"resource/components/player_component.hpp"

Arcadia::PlayerComponent::PlayerComponent(const nlohmann::json& json)
{}

auto Arcadia::PlayerComponent::ToJson() const -> nlohmann::json
{
    return nlohmann::json();
}

auto Arcadia::PlayerComponent::OnSnapshot() const -> std::shared_ptr<MementoDataBase>
{
    return std::shared_ptr<MementoDataBase>();
}

void Arcadia::PlayerComponent::OnRestore(const std::shared_ptr<MementoDataBase>& memento_data)
{}