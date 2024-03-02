#include "pch.hpp"
#include "meta_componts.hpp"

arcadia::tag_component::tag_component(const nlohmann::json& json):
    _tag(json.dump())
{}

auto arcadia::tag_component::to_json() const -> nlohmann::json
{
    return nlohmann::json(_tag);
}

