#include "pch.hpp"

#include "scene.hpp"

#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/transform_component.hpp"

Scene::Scene(const nlohmann::json& json) :
    Name(json.at("name"))
{
    const auto& json_entities = json.at("entities");

    for(const auto& [entity_index_str, json_entity] : json_entities.items())
    {
        auto& entity_info = CreateEntity(json_entity.at("name"), json_entity.value("type", ""s));
        entity_info.Display = json_entity.value("display", true);
        entity_info.Internal = json_entity.value("internal", false);

        const auto& json_comps = json_entity.at("components");

        // For components
        for(const auto& [json_comp_type_str, json_comp] : json_comps.items())
        {
            const auto& name = entity_info.GetName();
            Match<void>(
                json_comp_type_str,
                ModelComponent::GetTypeStringStatic(),
                [&]()
            {
                EmplaceComponent<ModelComponent>(name, json_comp).Snapshot();
            },
                LightComponent::GetTypeStringStatic(),
                [&]()
            {
                EmplaceComponent<LightComponent>(name, json_comp).Snapshot();
            },
                CameraComponent::GetTypeStringStatic(),
                [&]()
            {
                EmplaceComponent<CameraComponent>(name, json_comp).Snapshot();
            },
                PhysicsComponent::GetTypeStringStatic(),
                [&]()
            {
                EmplaceComponent<PhysicsComponent>(name, json_comp).Snapshot();
            },
                TransformComponent::GetTypeStringStatic(),
                [&]()
            {
                EmplaceComponent<TransformComponent>(name, json_comp).Snapshot();
            }
            );
        }
    }
}

auto Scene::ToJson() const -> nlohmann::json
{
    auto json_entities = nlohmann::json::array();

    for(const auto& [name, entity_info] : _EntityInfoStorage)
    {
        json_entities.push_back(
            {
                {"name",name},
                {"type",entity_info.Type},
                {"display",entity_info.Display},
                {"internal",entity_info.Internal},
                {"components",_CreateJsonComponents(name)}
            }
        );
    }

    nlohmann::json json{
        {"name",Name},
        {"entities",json_entities}
    };

    return json;
}

void Scene::RenameEntity(const std::string& name, const std::string& new_name)
{
    ACDA_ASSERT(ContainsEntity(name));
    ACDA_ASSERT(!ContainsEntity(new_name));

    auto node = _EntityInfoStorage.extract(name);
    node.key() = new_name;
    _EntityInfoStorage.insert(std::move(node));
}

auto Scene::ContainsEntity(const std::string& name) const -> bool
{
    return _EntityInfoStorage.find(name) != _EntityInfoStorage.end();
}

auto Scene::GetSize() const -> size_t
{
    return _EntityInfoStorage.size();
}

auto Scene::CountEntity(const std::function<bool(const std::string&, const EntityInfo&)>& pred) const -> size_t
{
    size_t count = 0;
    for(const auto& [name, entity_info] : _EntityInfoStorage)
    {
        if(pred(name, entity_info))
        {
            ++count;
        }
    }
    return count;
}

auto Scene::GetEntityInfo(const std::string& name) const -> const EntityInfo&
{
    ACDA_ASSERT(ContainsEntity(name));
    return _EntityInfoStorage.at(name);
}

auto Scene::GetEntityInfo(const std::string& name) -> EntityInfo&
{
    ACDA_ASSERT(ContainsEntity(name));
    return _EntityInfoStorage.at(name);
}

auto Scene::CreateEntity(const std::string& name, const std::string& type) -> EntityInfo&
{
    auto entity = _Registry.create();

    EntityInfo entity_info{ name, entity };
    entity_info.Type = type;

    _EntityInfoStorage.try_emplace(
        name,
        std::move(entity_info)
    );

    return _EntityInfoStorage.at(name);
}

void Scene::DestroyEntity(const std::string& name)
{
    ACDA_ASSERT(ContainsEntity(name));

    _Registry.destroy(_GetEntity(name));
    _EntityInfoStorage.erase(name);
}

auto Scene::_GetEntity(const std::string& name) const -> entt::entity
{
    ACDA_ASSERT(ContainsEntity(name));
    return _EntityInfoStorage.at(name).GetEntity();
}

auto Scene::_CreateJsonComponents(const std::string& name) const -> nlohmann::json
{
    auto json_comps = nlohmann::json::object();

    if(ContainsAllComponents<CameraComponent>(name))
    {
        json_comps.push_back({ CameraComponent::GetTypeStringStatic(),GetComponent<CameraComponent>(name).ToJson() });
    }
    if(ContainsAllComponents<LightComponent>(name))
    {
        json_comps.push_back({ LightComponent::GetTypeStringStatic(),GetComponent<LightComponent>(name).ToJson() });
    }
    if(ContainsAllComponents<ModelComponent>(name))
    {
        json_comps.push_back({ ModelComponent::GetTypeStringStatic(),GetComponent<ModelComponent>(name).ToJson() });
    }
    if(ContainsAllComponents<PhysicsComponent>(name))
    {
        json_comps.push_back({ PhysicsComponent::GetTypeStringStatic(),GetComponent<PhysicsComponent>(name).ToJson() });
    }
    if(ContainsAllComponents<TransformComponent>(name))
    {
        json_comps.push_back({ TransformComponent::GetTypeStringStatic(), GetComponent<TransformComponent>(name).ToJson() });
    }

    return json_comps;
}

auto EntityInfo::GetName() const -> const std::string&
{
    return _Name;
}

auto EntityInfo::GetEntity() const -> entt::entity
{
    return _Entity;
}