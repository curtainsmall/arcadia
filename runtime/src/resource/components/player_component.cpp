#include"pch.hpp"

#include"resource/components/player_component.hpp"

PlayerComponent::PlayerComponent(const nlohmann::json& json)
{}

auto PlayerComponent::to_json() const -> nlohmann::json
{
    return nlohmann::json();
}

auto PlayerComponent::on_snapshot() const -> std::shared_ptr<MementoDataBase>
{
    return std::shared_ptr<MementoDataBase>();
}

void PlayerComponent::on_restore(const std::shared_ptr<MementoDataBase>& memento_data)
{}
