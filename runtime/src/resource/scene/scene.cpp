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
        const auto entity = create(entity_name);

        // For other components
        for(const auto& [json_comp_type_str, json_comp] : json_entities.at(entity_name).items())
        {
            arcadia::match<void>(
                json_comp_type_str,
                "model"s,
                [&]() -> void
            {
                emplace<arcadia::model_component>(entity, json_comp);
            },
                "camera"s,
                [&]() -> void
            {
                emplace<arcadia::camera_component>(entity, json_comp);
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

    auto name_comp_view =_registry.view<arcadia::name_component>();
    for(auto entity : name_comp_view)
    {
        const auto& [name_comp] = name_comp_view.get(entity);
        json
            .at("entities")
            .push_back(
                { name_comp.get(), nlohmann::json::object() }
        );
    }

    json_scene_add_component_to_entity_helper{ *this,json.at("entities") }
        .add<arcadia::model_component>("model")
        .add<arcadia::camera_component>("camera");

    return json;
}

auto arcadia::scene::create(const std::string& name) -> entt::entity
{
    if(_name_uset.contains(name))
    {
        throw conflict_name{ std::format("Conflict entity name: {}",name) };
    }

    auto entity = _registry.create();
    emplace<arcadia::name_component>(entity, name);
    set_modified(true);
    return entity;
}

auto arcadia::scene::destroy(entt::entity entity) -> entt::registry::version_type
{
    set_modified(true);
    return _registry.destroy(entity);
}

auto arcadia::scene::valid(const entt::entity entity) const -> bool
{
    return _registry.valid(entity);
}

void arcadia::scene::_check_valid_entity_or_throw(const entt::entity entity) const
{
    if(!valid(entity))
    {
        throw invalid_entity{ std::format("Invalid entity: {}",static_cast<entt::id_type>(entity)) };
    }
}
