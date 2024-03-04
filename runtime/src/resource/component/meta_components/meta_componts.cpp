#include "pch.hpp"
#include "meta_componts.hpp"

arcadia::name_component::name_component(const nlohmann::json& json):
    _name(json.dump())
{}

auto arcadia::name_component::to_json() const -> nlohmann::json
{
    return nlohmann::json(_name);
}

