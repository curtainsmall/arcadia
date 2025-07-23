#include "scene.hpp"

#include "core/assert.hpp"
#include "core/match.hpp"
#include "resource/components/camera_component.hpp"
#include "resource/components/light_component.hpp"
#include "resource/components/model_component.hpp"
#include "resource/components/physics_component.hpp"
#include "resource/components/transform_component.hpp"

Arcadia::Scene::Scene(const std::string& name):
    _Name(name)
{
}

Arcadia::Scene::Scene(const nlohmann::json& json):
    _Name(json.at("name"))
{
    const nlohmann::json& json_entities = json.at("entities");

    for(const auto& [entity_index_string, json_entity] : json_entities.items())
    {
        EntityId entity_id = CreateEntity(json_entity.at("name"), json_entity.value("type", std::string{}));
        EntityInfo& entity_info = GetEntityInfo(entity_id);
        entity_info.Displayed = json_entity.value("display", true);
        entity_info.Internal = json_entity.value("internal", false);

        const nlohmann::json& json_comps = json_entity.at("components");

        // For components
        for(const auto& [json_comp_type_str, json_comp] : json_comps.items())
        {
            Match<void>(
                json_comp_type_str,
                ModelComponent::GetTypeStringStatic(),
                [&]()
                {
                    EmplaceComponent<ModelComponent>(entity_id, json_comp);
                },
                LightComponent::GetTypeStringStatic(),
                [&]()
                {
                    EmplaceComponent<LightComponent>(entity_id, json_comp);
                },
                CameraComponent::GetTypeStringStatic(),
                [&]()
                {
                    EmplaceComponent<CameraComponent>(entity_id, json_comp);
                },
                PhysicsComponent::GetTypeStringStatic(),
                [&]()
                {
                    EmplaceComponent<PhysicsComponent>(entity_id, json_comp);
                },
                TransformComponent::GetTypeStringStatic(),
                [&]()
                {
                    EmplaceComponent<TransformComponent>(entity_id, json_comp);
                }
            );
        }
    }
}

auto Arcadia::Scene::ToJson() const -> nlohmann::json
{
    nlohmann::json json_entities = nlohmann::json::array();

    for(const auto& [entity_id, entity] : _EntityInfoStorage)
    {
        json_entities.push_back(
            {
                {"name",entity._Name},
                {"type",entity.TypeString},
                {"display",entity.Displayed},
                {"internal",entity.Internal},
                {"components",_CreateJsonComponents(entity_id)}
            }
        );
    }

    nlohmann::json json{
        {"name",_Name},
        {"entities",json_entities}
    };

    return json;
}

auto Arcadia::Scene::GetName() const -> const std::string&
{
    return _Name;
}

void Arcadia::Scene::SetName(const std::string& name)
{
    _Name = name;
}

auto Arcadia::Scene::ContainsEntity(EntityId entity_id) const -> bool
{
    return _EntityInfoStorage.contains(entity_id);
}

auto Arcadia::Scene::GetEntityCount() const -> std::size_t
{
    return _EntityInfoStorage.size();
}

auto Arcadia::Scene::GetEntityCount(const std::function<bool(EntityId, const EntityInfo&)>& pred) const -> std::size_t
{
    std::size_t count = 0;
    for(const auto& [entity_id, entity] : _EntityInfoStorage)
    {
        if(pred(entity_id, entity))
        {
            ++count;
        }
    }
    return count;
}

auto Arcadia::Scene::GetEntityInfo(EntityId entity_id) const -> const EntityInfo&
{
    ACDA_ASSERT(ContainsEntity(entity_id));
    return _EntityInfoStorage.at(entity_id);
}

auto Arcadia::Scene::GetEntityInfo(EntityId entity_id) -> EntityInfo&
{
    ACDA_ASSERT(ContainsEntity(entity_id));
    return _EntityInfoStorage.at(entity_id);
}

auto Arcadia::Scene::CreateEntity(const std::string& entity_name, const std::string& type_string) -> EntityId
{
    ACDA_ASSERT(!IsEntityNameUsed(entity_name));

    EntityId entity_id = _Registry.create();

    EntityInfo entity_info;
    entity_info._Name = entity_name;
    entity_info.TypeString = type_string;

    _EntityInfoStorage.insert_or_assign(
        entity_id,
        std::move(entity_info)
    );

    _EntityNameToEntityIdLookupMap.insert_or_assign(
        entity_name,
        entity_id
    );

    return entity_id;
}

void Arcadia::Scene::DestroyEntity(EntityId entity_id)
{
    ACDA_ASSERT(ContainsEntity(entity_id));

    _EntityNameToEntityIdLookupMap.erase(GetEntityInfo(entity_id).GetName());
    _Registry.destroy(entity_id);
    _EntityInfoStorage.erase(entity_id);
}

void Arcadia::Scene::RenameEntity(EntityId entity_id, const std::string& new_entity_name)
{
    ACDA_ASSERT(ContainsEntity(entity_id));
    ACDA_ASSERT(!IsEntityNameUsed(new_entity_name));

    EntityInfo& entity_info = GetEntityInfo(entity_id);
    const std::string old_name = entity_info._Name;
    entity_info._Name = new_entity_name;

    _EntityNameToEntityIdLookupMap.insert_or_assign(
        new_entity_name,
        _EntityNameToEntityIdLookupMap.at(old_name)
    );
    _EntityNameToEntityIdLookupMap.erase(old_name);
}

auto Arcadia::Scene::_CreateJsonComponents(EntityId entity_id) const -> nlohmann::json
{
    nlohmann::json json_comps = nlohmann::json::object();

    if(ContainsAllComponents<CameraComponent>(entity_id))
    {
        json_comps.push_back({ CameraComponent::GetTypeStringStatic(),GetComponent<CameraComponent>(entity_id).ToJson() });
    }
    if(ContainsAllComponents<LightComponent>(entity_id))
    {
        json_comps.push_back({ LightComponent::GetTypeStringStatic(),GetComponent<LightComponent>(entity_id).ToJson() });
    }
    if(ContainsAllComponents<ModelComponent>(entity_id))
    {
        json_comps.push_back({ ModelComponent::GetTypeStringStatic(),GetComponent<ModelComponent>(entity_id).ToJson() });
    }
    if(ContainsAllComponents<PhysicsComponent>(entity_id))
    {
        json_comps.push_back({ PhysicsComponent::GetTypeStringStatic(),GetComponent<PhysicsComponent>(entity_id).ToJson() });
    }
    if(ContainsAllComponents<TransformComponent>(entity_id))
    {
        json_comps.push_back({ TransformComponent::GetTypeStringStatic(), GetComponent<TransformComponent>(entity_id).ToJson() });
    }

    return json_comps;
}

auto Arcadia::Scene::GetEntityInfoStorage() -> EntityInfoStorageType&
{
    return _EntityInfoStorage;
}

auto Arcadia::Scene::IsEntityNameUsed(const std::string& entity_name) const -> bool
{
    return _EntityNameToEntityIdLookupMap.contains(entity_name);
}

auto Arcadia::Scene::GetEntityIdByName(const std::string& entity_name) const -> EntityId
{
    return _EntityNameToEntityIdLookupMap.at(entity_name);
}

auto Arcadia::EntityInfo::GetName() const -> const std::string&
{
    return _Name;
}