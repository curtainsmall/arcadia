#include"pch.hpp"

#include"resource/components/player_component.hpp"

PlayerComponent::PlayerComponent(const nlohmann::json& json)
{}

auto PlayerComponent::ToJson() const -> nlohmann::json
{
    return nlohmann::json();
}

auto PlayerComponent::OnSnapshot() const -> std::shared_ptr<MementoDataBase>
{
    return std::shared_ptr<MementoDataBase>();
}

void PlayerComponent::OnRestore(const std::shared_ptr<MementoDataBase>& memento_data)
{}
