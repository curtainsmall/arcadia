#include "pch.hpp"
#include "physics_component.hpp"

arcadia::physics_component::physics_component(const nlohmann::json& json)
{}

auto arcadia::physics_component::to_json() const -> nlohmann::json
{
    return nlohmann::json();
}
