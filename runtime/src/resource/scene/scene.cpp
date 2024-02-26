#include "pch.hpp"
#include "scene.hpp"

#include"resource/component/meta_components/meta_componts.hpp"

auto arcadia::scene::create_entity(const std::string& name) -> entt::entity
{
    auto entity = _registry.create();
    emplace_component<arcadia::tag_component>(entity, name);
    return entity;
}

auto arcadia::scene::destroy_entity(entt::entity entity) -> entt::registry::version_type
{
    return _registry.destroy(entity);
}

auto arcadia::scene::is_valid(const entt::entity entity) const -> bool
{
    return _registry.valid(entity);
}

void arcadia::scene::_check_valid_entity_or_throw(const entt::entity entity) const
{
    if(!is_valid(entity))
    {
        throw invalid_entity{ std::format("Invalid entity: {}",static_cast<entt::id_type>(entity)) };
    }
}
