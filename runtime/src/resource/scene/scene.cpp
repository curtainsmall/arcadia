#include "pch.hpp"
#include "scene.hpp"

#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/light_component/light_component.hpp"
#include"resource/component/model_component/model_component.hpp"
#include"resource/component/physics_component/physics_component.hpp"

Arcadia::EntityInfo::EntityInfo(const nlohmann::json& json):
    ShouldRenderInViewport(json.at("should_render_in_viewport"))
{}

auto Arcadia::EntityInfo::ToJson() const -> nlohmann::json
{
    return nlohmann::json{
        {"should_render_in_viewport", ShouldRenderInViewport}
    };
}

Arcadia::Scene::Scene(const nlohmann::json& json):
    _Name(json.at("name"))
{
    const auto& json_entities = json.at("entities");

    for(const auto& [entity_index_str, json_entity] : json_entities.items())
    {
        const auto entity = Create(json_entity.at("name"), json_entity.at("info"));
        const auto& json_comps = json_entity.at("components");

        // For components
        for(const auto& [json_comp_type_str, json_comp] : json_comps.items())
        {
            Arcadia::Match<void>(
                json_comp_type_str,
                Arcadia::ModelComponent::GetTypeStrStatic(),
                [&]()
            {
                Emplace<Arcadia::ModelComponent>(entity, json_comp);
            },
                Arcadia::LightComponent::GetTypeStrStatic(),
                [&]()
            {
                Emplace<Arcadia::LightComponent>(entity, json_comp);
            },
                Arcadia::CameraComponent::GetTypeStrStatic(),
                [&]()
            {
                Emplace<Arcadia::CameraComponent>(entity, json_comp);
            },
                Arcadia::PhysicsComponent::GetTypeStrStatic(),
                [&]()
            {
                Emplace<Arcadia::PhysicsComponent>(entity, json_comp);
            }
            );
        }

    }
}

auto Arcadia::Scene::ToJson() const -> nlohmann::json
{
    auto json_entities = nlohmann::json::array();
    for(const auto& [name, entity] : _NameEntityBimap)
    {
        const auto& entity_info = _EntityInfoStorage.at(entity);

        nlohmann::json json_entity{
            {"name", name},
            {"info", entity_info.ToJson()},
            {"components",_CreateJsonComponents(entity)}
        };

        json_entities.push_back(json_entity);
    }

    nlohmann::json json{
        {"name",GetName()},
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

auto Arcadia::Scene::GetNameOfEntity(const entt::entity entity) const -> const std::string&
{
    return _NameEntityBimap.right.at(entity);
}

auto Arcadia::Scene::GetEntityOfName(const std::string& name) const -> entt::entity
{
    return _NameEntityBimap.left.at(name);
}

auto Arcadia::Scene::GetEntityInfo(const entt::entity entity) const -> const Arcadia::EntityInfo&
{
    ARCADIA_ASSERT(_EntityInfoStorage.contains(entity));

    return _EntityInfoStorage.at(entity);
}

auto Arcadia::Scene::GetEntityInfo(const entt::entity entity) -> Arcadia::EntityInfo&
{
    ARCADIA_ASSERT(_EntityInfoStorage.contains(entity));

    auto& entity_info = _EntityInfoStorage.at(entity);
    return entity_info;
}

auto Arcadia::Scene::Rename(const std::string& old_name, const std::string& new_name) -> bool
{
    if(Contains(new_name))
    {
        return false;
    }

    _NameEntityBimap.left.replace_key(_NameEntityBimap.left.find(old_name), new_name);
    return true;
}

auto Arcadia::Scene::Contains(const std::string& name) const -> bool
{
    return _NameEntityBimap.left.find(name) != _NameEntityBimap.left.end();
}

auto Arcadia::Scene::Create(const std::string& name) -> entt::entity
{
    auto entity = _Registry.create();
    _NameEntityBimap.left.insert(std::make_pair(name, entity));
    _EntityInfoStorage.try_emplace(entity);
    return entity;
}

auto Arcadia::Scene::Create(const std::string& name, const nlohmann::json& json_entity_info) -> entt::entity
{
    auto entity = _Registry.create();
    _NameEntityBimap.left.insert(std::make_pair(name, entity));
    _EntityInfoStorage.try_emplace(entity, json_entity_info);
    return entity;
}

auto Arcadia::Scene::Destroy(entt::entity entity) -> entt::registry::version_type
{
    auto version = _Registry.destroy(entity);
    _NameEntityBimap.right.erase(entity);
    _EntityInfoStorage.erase(entity);
    return version;
}

auto Arcadia::Scene::IsValid(const entt::entity entity) const -> bool
{
    return _Registry.valid(entity);
}

auto Arcadia::Scene::_CreateJsonComponents(const entt::entity entity) const -> nlohmann::json
{
    auto json_comps = nlohmann::json::object();

    if(AllOf<Arcadia::CameraComponent>(entity))
    {
        json_comps.push_back({ Arcadia::CameraComponent::GetTypeStrStatic(),Get<Arcadia::CameraComponent>(entity).ToJson() });
    }
    if(AllOf<Arcadia::LightComponent>(entity))
    {
        json_comps.push_back({ Arcadia::LightComponent::GetTypeStrStatic(),Get<Arcadia::LightComponent>(entity).ToJson() });
    }
    if(AllOf<Arcadia::ModelComponent>(entity))
    {
        json_comps.push_back({ Arcadia::ModelComponent::GetTypeStrStatic(),Get<Arcadia::ModelComponent>(entity).ToJson() });
    }
    if(AllOf<Arcadia::PhysicsComponent>(entity))
    {
        json_comps.push_back({ Arcadia::PhysicsComponent::GetTypeStrStatic(),Get<Arcadia::PhysicsComponent>(entity).ToJson() });
    }

    return json_comps;
}


