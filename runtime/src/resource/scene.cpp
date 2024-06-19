#include "pch.hpp"

#include "scene.hpp"

#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/transform_component.hpp"

Scene::Scene(const nlohmann::json& json):
    name(json.at("name"))
{
    const auto& json_entities = json.at("entities");

    for(const auto& [entity_index_str, json_entity] : json_entities.items())
    {
        auto& entity_info = create(json_entity.at("name"), json_entity.value("type", ""s));
        entity_info.display = json_entity.value("display", true);
        entity_info.internal = json_entity.value("internal", false);

        const auto& json_comps = json_entity.at("components");

        // For components
        for(const auto& [json_comp_type_str, json_comp] : json_comps.items())
        {
            const auto& name = entity_info.get_name();
            match<void>(
                json_comp_type_str,
                ModelComponent::get_type_str_static(),
                [&]()
            {
                emplace<ModelComponent>(name, json_comp).snapshot();
            },
                LightComponent::get_type_str_static(),
                [&]()
            {
                emplace<LightComponent>(name, json_comp).snapshot();
            },
                CameraComponent::get_type_str_static(),
                [&]()
            {
                emplace<CameraComponent>(name, json_comp).snapshot();
            },
                PhysicsComponent::get_type_str_static(),
                [&]()
            {
                emplace<PhysicsComponent>(name, json_comp).snapshot();
            },
                TransformComponent::get_type_str_static(),
                [&]()
            {
                emplace<TransformComponent>(name, json_comp).snapshot();
            }
            );
        }

    }
}

auto Scene::to_json() const -> nlohmann::json
{
    auto json_entities = nlohmann::json::array();

    for(const auto& [name, entity_info] : _entity_info_storage)
    {
        json_entities.push_back(
            {
                {"name",name},
                {"type",entity_info.type},
                {"display",entity_info.display},
                {"internal",entity_info.internal},
                {"components",_create_json_components(name)}
            }
        );
    }

    nlohmann::json json{
        {"name",name},
        {"entities",json_entities}
    };

    return json;
}

void Scene::rename(const std::string& name, const std::string& new_name)
{
    ACDA_ASSERT(contains(name));
    ACDA_ASSERT(!contains(new_name));

    auto node = _entity_info_storage.extract(name);
    node.key() = new_name;
    _entity_info_storage.insert(std::move(node));

}

auto Scene::contains(const std::string& name) const -> bool
{
    return _entity_info_storage.find(name) != _entity_info_storage.end();
}

auto Scene::size() const -> std::size_t
{
    return _entity_info_storage.size();
}

auto Scene::count(const std::function<bool(const std::string&, const EntityInfo&)>& pred) const -> std::size_t
{
    std::size_t count = 0;
    for(const auto& [name, entity_info] : _entity_info_storage)
    {
        if(pred(name, entity_info))
        {
            ++count;
        }
    }
    return count;
}

auto Scene::entity_info(const std::string& name) const -> const EntityInfo&
{
    ACDA_ASSERT(contains(name));
    return _entity_info_storage.at(name);
}

auto Scene::entity_info(const std::string& name) -> EntityInfo&
{
    ACDA_ASSERT(contains(name));
    return _entity_info_storage.at(name);
}

auto Scene::create(const std::string& name, const std::string& type) -> EntityInfo&
{
    auto entity = _registry.create();

    EntityInfo entity_info{ name, entity };
    entity_info.type = type;

    _entity_info_storage.try_emplace(
        name,
        std::move(entity_info)
    );

    return _entity_info_storage.at(name);
}

void Scene::destroy(const std::string& name)
{
    ACDA_ASSERT(contains(name));

    _registry.destroy(_entity_of(name));
    _entity_info_storage.erase(name);
}

auto Scene::_entity_of(const std::string& name) const -> entt::entity
{
    ACDA_ASSERT(contains(name));
    return _entity_info_storage.at(name).get_entity();
}

auto Scene::_create_json_components(const std::string& name) const -> nlohmann::json
{
    auto json_comps = nlohmann::json::object();

    if(all_of<CameraComponent>(name))
    {
        json_comps.push_back({ CameraComponent::get_type_str_static(),get<CameraComponent>(name).to_json() });
    }
    if(all_of<LightComponent>(name))
    {
        json_comps.push_back({ LightComponent::get_type_str_static(),get<LightComponent>(name).to_json() });
    }
    if(all_of<ModelComponent>(name))
    {
        json_comps.push_back({ ModelComponent::get_type_str_static(),get<ModelComponent>(name).to_json() });
    }
    if(all_of<PhysicsComponent>(name))
    {
        json_comps.push_back({ PhysicsComponent::get_type_str_static(),get<PhysicsComponent>(name).to_json() });
    }
    if(all_of<TransformComponent>(name))
    {
        json_comps.push_back({ TransformComponent::get_type_str_static(), get<TransformComponent>(name).to_json() });
    }

    return json_comps;
}

auto EntityInfo::get_name() const -> const std::string&
{
    return _name;
}

auto EntityInfo::get_entity() const -> entt::entity
{
    return _entity;
}
