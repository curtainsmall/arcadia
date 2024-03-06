#include "pch.hpp"
#include "scene.hpp"

#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/model_component/model_component.hpp"

/* Json of scene is in pattern:
    {
        "name": <scene_name>,
        "entity":
        {
            "<entity1>":
            {
                "tag": <tag_component_json_object1>,
                "model": <model_component_json_object>,
                ...
            },
            "<entity2>":
            {
                "tag": <tag_component_json_object2>,
                "camera": <camera_component_json_object>,
                ...
            },
            ...
        }
    }
    where *_component_json_object(s) are optional except tag_component_json_object.
    For c++ struct `*_component` where `*` is the actual type name, `*` is used as the key in JSON object.
    For example, JSON object key for `tag_component` is `tag`.
*/

arcadia::scene::scene(const nlohmann::json& json):
    _name(json.at("name"))
{

    const auto& json_entities = json.at("entities");

    for(const auto& [entity_name, json_comps] : json_entities.items())
    {
        // All components bound to `json_entity_str` should now be bound to `entity` in the new scene
        const auto entity = create_entity(entity_name);

        // For other components
        for(const auto& [json_comp_type_str, json_comp] : json_entities.at(entity_name).items())
        {
            arcadia::match<void>(
                json_comp_type_str,
                "model"s,
                [&]() -> void
            {
                emplace_component<arcadia::model_component>(entity, json_comp);
            },
                "camera"s,
                [&]() -> void
            {
                emplace_component<arcadia::camera_component>(entity, json_comp);
            }
            );
        }

    }
}

auto arcadia::scene::to_json() const -> nlohmann::json
{
    nlohmann::json json{
        {"name",_name},
        {"entities",nlohmann::json::object()}
    };

    for(const auto& [name, entity] : _name_entity_bimap)
    {
        json
            .at("entities")
            .push_back(
                { name, nlohmann::json::object() }
        );
    }

    json_scene_add_component_to_entity_helper{ *this,json.at("entities") }
        .add<arcadia::model_component>("model")
        .add<arcadia::camera_component>("camera");

    return json;
}

auto arcadia::scene::create_entity(const std::string& name) -> entt::entity
{
    if(contains_entity(name))
    {
        throw conflict_name{ std::format("Conflict entity name: {}",name) };
    }

    auto entity = _registry.create();
    set_modified(true);
    _name_entity_bimap.left.insert(std::make_pair(name, entity));
    return entity;
}

auto arcadia::scene::destroy_entity(const std::string& name) -> entt::registry::version_type
{
    if(!contains_entity(name))
    {
        throw invalid_name{ std::format("Invalid entity name: {}", name) };
    }

    return destroy_entity(_name_entity_bimap.left.at(name));
}

auto arcadia::scene::destroy_entity(entt::entity entity) -> entt::registry::version_type
{
    set_modified(true);
    _name_entity_bimap.right.erase(entity);
    return _registry.destroy(entity);
}

auto arcadia::scene::is_entity_valid(const entt::entity entity) const -> bool
{
    return _registry.valid(entity);
}

auto arcadia::scene::contains_entity(const std::string& name) const -> bool
{
    return _name_entity_bimap.left.find(name) != _name_entity_bimap.left.end();
}

auto arcadia::scene::rename_entity(const std::string& old_name, const std::string& new_name) -> bool
{
    if(contains_entity(new_name))
    {
        return false;
    }

    _name_entity_bimap.left.replace_key(_name_entity_bimap.left.find(old_name), new_name);
    set_modified(true);
    return true;
}

void arcadia::scene::_check_valid_entity_or_throw(const entt::entity entity) const
{
    if(!is_entity_valid(entity))
    {
        throw invalid_entity{ std::format("Invalid entity: {}",static_cast<entt::id_type>(entity)) };
    }
}
