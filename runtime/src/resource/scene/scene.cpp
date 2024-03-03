#include "pch.hpp"
#include "scene.hpp"

#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/meta_components/meta_componts.hpp"
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
    auto& json_entity = json.at("entity");

    for(const auto& [json_entity_str, json_comps] : json_entity.items())
    {
        // For tag component
        // All components bound to `json_entity_str` should now be bound to `entity` in the new scene
        auto entity = create_entity(json_entity.at(json_entity_str).at("tag").dump());

        // For other components
        for(const auto& [json_comp_key, json_comp] : json_entity.at(json_entity_str).items())
        {
            arcadia::match<void>(
                json_comp_key,
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
        {"entity",nlohmann::json::object()}
    };

    auto tag_comp_view =_registry.view<arcadia::tag_component>();
    for(auto entity : tag_comp_view)
    {
        const auto& [tag_comp] = tag_comp_view.get(entity);
        json
            .at("entity")
            .push_back(
                { arcadia::to_string(entity),{
                    { "tag", tag_comp.to_json()}
                    }
                }
        );
    }

    json_scene_add_component_to_entity_helper{ *this,json.at("entity") }
        .add<arcadia::model_component>("model")
        .add<arcadia::camera_component>("camera");

    return json;
}

auto arcadia::scene::create_entity(const std::string& name) -> entt::entity
{
    auto entity = _registry.create();
    emplace_component<arcadia::tag_component>(entity, name);
    _modified = true;
    return entity;
}

auto arcadia::scene::destroy_entity(entt::entity entity) -> entt::registry::version_type
{
    _modified = true;
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
