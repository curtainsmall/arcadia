#include "pch.hpp"
#include "scene.hpp"

#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/light_component/light_component.hpp"
#include"resource/component/model_component/model_component.hpp"
#include"resource/component/physics_component/physics_component.hpp"


/* Json object of scene is in pattern:
    {
        "name": <scene_name>,
        "entities":
        {
            "<entity_name_1>":
            {
                <component_type_str>: <component_json_object>,
                ...
            },
            "<entity_name_2>":
            {
                <component_type_str>: <component_json_object>,
                ...
            },
            ...
        }
    }
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
                arcadia::model_component::get_type_str_static(),
                [&]()
            {
                emplace_component<arcadia::model_component>(entity, json_comp);
            },
                arcadia::light_component::get_type_str_static(),
                [&]()
            {
                emplace_component<arcadia::light_component>(entity, json_comp);
            },
                arcadia::camera_component::get_type_str_static(),
                [&]()
            {
                emplace_component<arcadia::camera_component>(entity, json_comp);
            },
                arcadia::physics_component::get_type_str_static(),
                [&]()
            {
                emplace_component<arcadia::physics_component>(entity, json_comp);
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
        .add<arcadia::camera_component>(arcadia::camera_component::get_type_str_static())
        .add<arcadia::light_component>(arcadia::light_component::get_type_str_static())
        .add<arcadia::model_component>(arcadia::model_component::get_type_str_static())
        .add<arcadia::physics_component>(arcadia::physics_component::get_type_str_static());

    return json;
}

auto arcadia::scene::get_name_of_entity(const entt::entity entity) const -> const std::string&
{
    auto iter = _name_entity_bimap.right.find(entity);
    if(iter == _name_entity_bimap.right.end())
    {
        throw invalid_entity{ std::format("Cannot find name of entity {}, because it is invalid", entity) };
    }

    return iter->get_left();
}

auto arcadia::scene::get_entity_of_name(const std::string& name) const -> entt::entity
{
    auto iter = _name_entity_bimap.left.find(name);
    if(iter == _name_entity_bimap.left.end())
    {
        throw invalid_name{ std::format("Cannot find entity of name {}, because it is invalid",name) };
    }

    return iter->get_right();
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

auto arcadia::scene::contains_entity(const entt::entity entity) const -> bool
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
    if(!contains_entity(entity))
    {
        throw invalid_entity{ std::format("Invalid entity: {}",entity) };
    }
}
