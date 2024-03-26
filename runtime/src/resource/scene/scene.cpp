#include "pch.hpp"
#include "scene.hpp"

#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/light_component/light_component.hpp"
#include"resource/component/model_component/model_component.hpp"
#include"resource/component/physics_component/physics_component.hpp"

arcadia::entity_info::entity_info(const nlohmann::json& json):
    should_render_in_viewport(json.at("should_render_in_viewport"))
{}

auto arcadia::entity_info::to_json() const -> nlohmann::json
{
    return nlohmann::json{
        {"should_render_in_viewport", should_render_in_viewport}
    };
}

arcadia::scene::scene(const nlohmann::json& json):
    _name(json.at("name"))
{
    const auto& json_entities = json.at("entities");

    for(const auto& [entity_index_str, json_entity] : json_entities.items())
    {
        const auto entity = create(json_entity.at("name"), json_entity.at("info"));
        const auto& json_comps = json_entity.at("components");

        // For components
        for(const auto& [json_comp_type_str, json_comp] : json_comps.items())
        {
            arcadia::match<void>(
                json_comp_type_str,
                arcadia::model_component::get_type_str_static(),
                [&]()
            {
                emplace<arcadia::model_component>(entity, json_comp);
            },
                arcadia::light_component::get_type_str_static(),
                [&]()
            {
                emplace<arcadia::light_component>(entity, json_comp);
            },
                arcadia::camera_component::get_type_str_static(),
                [&]()
            {
                emplace<arcadia::camera_component>(entity, json_comp);
            },
                arcadia::physics_component::get_type_str_static(),
                [&]()
            {
                emplace<arcadia::physics_component>(entity, json_comp);
            }
            );
        }

    }
}

auto arcadia::scene::to_json() const -> nlohmann::json
{
    auto json_entities = nlohmann::json::array();
    for(const auto& [name, entity] : _name_entity_bimap)
    {
        const auto& entity_info = _entity_info_umap.at(entity);

        nlohmann::json json_entity{
            {"name", name},
            {"info", entity_info.to_json()},
            {"components",_create_json_components(entity)}
        };

        json_entities.push_back(json_entity);
    }

    nlohmann::json json{
        {"name",_name},
        {"entities",json_entities}
    };

    return json;
}


auto arcadia::scene::get_name_of_entity(const entt::entity entity) const -> const std::string&
{
    return _name_entity_bimap.right.at(entity);
}

auto arcadia::scene::get_entity_of_name(const std::string& name) const -> entt::entity
{
    return _name_entity_bimap.left.at(name);
}

auto arcadia::scene::get_entity_info(const entt::entity entity) const -> const arcadia::entity_info&
{
    ARCADIA_ASSERT(_entity_info_umap.contains(entity));

    return _entity_info_umap.at(entity);
}

auto arcadia::scene::get_entity_info(const entt::entity entity) -> arcadia::entity_info&
{
    ARCADIA_ASSERT(_entity_info_umap.contains(entity));

    auto& entity_info = _entity_info_umap.at(entity);
    set_modified(true);
    return entity_info;
}

auto arcadia::scene::rename(const std::string& old_name, const std::string& new_name) -> bool
{
    if(contains(new_name))
    {
        return false;
    }

    _name_entity_bimap.left.replace_key(_name_entity_bimap.left.find(old_name), new_name);
    set_modified(true);
    return true;
}

auto arcadia::scene::contains(const std::string& name) const -> bool
{
    return _name_entity_bimap.left.find(name) != _name_entity_bimap.left.end();
}

auto arcadia::scene::create(const std::string& name) -> entt::entity
{
    auto entity = _registry.create();
    set_modified(true);
    _name_entity_bimap.left.insert(std::make_pair(name, entity));
    _entity_info_umap.try_emplace(entity);
    return entity;
}

auto arcadia::scene::create(const std::string& name, const nlohmann::json& json_entity_info) -> entt::entity
{
    auto entity = _registry.create();
    set_modified(true);
    _name_entity_bimap.left.insert(std::make_pair(name, entity));
    _entity_info_umap.try_emplace(entity, json_entity_info);
    return entity;
}

auto arcadia::scene::destroy(entt::entity entity) -> entt::registry::version_type
{
    auto version = _registry.destroy(entity);
    set_modified(true);
    _name_entity_bimap.right.erase(entity);
    _entity_info_umap.erase(entity);
    return version;
}

auto arcadia::scene::is_valid(const entt::entity entity) const -> bool
{
    return _registry.valid(entity);
}

auto arcadia::scene::_create_json_components(const entt::entity entity) const -> nlohmann::json
{
    auto json_comps = nlohmann::json::object();

    if(all_of<arcadia::camera_component>(entity))
    {
        json_comps.push_back({ arcadia::camera_component::get_type_str_static(),get<arcadia::camera_component>(entity).to_json() });
    }
    if(all_of<arcadia::light_component>(entity))
    {
        json_comps.push_back({ arcadia::light_component::get_type_str_static(),get<arcadia::light_component>(entity).to_json() });
    }
    if(all_of<arcadia::model_component>(entity))
    {
        json_comps.push_back({ arcadia::model_component::get_type_str_static(),get<arcadia::model_component>(entity).to_json() });
    }
    if(all_of<arcadia::physics_component>(entity))
    {
        json_comps.push_back({ arcadia::physics_component::get_type_str_static(),get<arcadia::physics_component>(entity).to_json() });
    }

    return json_comps;
}


