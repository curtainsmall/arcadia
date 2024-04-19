#include "pch.hpp"

#include "scene.hpp"

#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/transform_component.hpp"

Arcadia::Scene::Scene(const nlohmann::json& json):
    Name(json.at("name"))
{
    const auto& json_entities = json.at("entities");

    for(const auto& [entity_index_str, json_entity] : json_entities.items())
    {
        auto& entity_info = Create(json_entity.at("name"), json_entity.value("type", ""s));
        entity_info.Display = json_entity.value("display", true);
        entity_info.Internal = json_entity.value("internal", false);

        const auto& json_comps = json_entity.at("components");

        // For components
        for(const auto& [json_comp_type_str, json_comp] : json_comps.items())
        {
            const auto& name = entity_info.Name;
            Arcadia::Match<void>(
                json_comp_type_str,
                Arcadia::ModelComponent::GetTypeStrStatic(),
                [&]()
            {
                Emplace<Arcadia::ModelComponent>(name, json_comp).Snapshot();
            },
                Arcadia::LightComponent::GetTypeStrStatic(),
                [&]()
            {
                Emplace<Arcadia::LightComponent>(name, json_comp).Snapshot();
            },
                Arcadia::CameraComponent::GetTypeStrStatic(),
                [&]()
            {
                Emplace<Arcadia::CameraComponent>(name, json_comp).Snapshot();
            },
                Arcadia::PhysicsComponent::GetTypeStrStatic(),
                [&]()
            {
                Emplace<Arcadia::PhysicsComponent>(name, json_comp).Snapshot();
            },
                Arcadia::TransformComponent::GetTypeStrStatic(),
                [&]()
            {
                Emplace<Arcadia::TransformComponent>(name, json_comp).Snapshot();
            }
            );
        }

    }
}

auto Arcadia::Scene::ToJson() const -> nlohmann::json
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

void Arcadia::Scene::Rename(const std::string& name, const std::string& new_name)
{
    ARCADIA_ASSERT(Contains(name));
    ARCADIA_ASSERT(!Contains(new_name));

    auto node = _EntityInfoStorage.extract(name);
    node.key() = new_name;
    _EntityInfoStorage.insert(std::move(node));

}

auto Arcadia::Scene::Contains(const std::string& name) const -> bool
{
    return _EntityInfoStorage.find(name) != _EntityInfoStorage.end();
}

auto Arcadia::Scene::Size() const -> std::size_t
{
    return _EntityInfoStorage.size();
}

auto Arcadia::Scene::GetEntityInfo(const std::string& name) const -> const Arcadia::EntityInfo&
{
    ARCADIA_ASSERT(Contains(name));
    return _EntityInfoStorage.at(name);
}

auto Arcadia::Scene::GetEntityInfo(const std::string& name) -> Arcadia::EntityInfo&
{
    ARCADIA_ASSERT(Contains(name));
    return _EntityInfoStorage.at(name);
}

auto Arcadia::Scene::Create(const std::string& name, const std::string& type) -> Arcadia::EntityInfo&
{
    auto entity = _Registry.create();

    Arcadia::EntityInfo entity_info{ name, entity };
    entity_info.Type = type;

    _EntityInfoStorage.try_emplace(
        name,
        std::move(entity_info)
    );

    return _EntityInfoStorage.at(name);
}

void Arcadia::Scene::Destroy(const std::string& name)
{
    ARCADIA_ASSERT(Contains(name));

    _Registry.destroy(_EntityOf(name));
    _EntityInfoStorage.erase(name);
}

auto Arcadia::Scene::_EntityOf(const std::string& name) const -> entt::entity
{
    ARCADIA_ASSERT(Contains(name));
    return _EntityInfoStorage.at(name).Entity;
}

auto Arcadia::Scene::_CreateJsonComponents(const std::string& name) const -> nlohmann::json
{
    auto json_comps = nlohmann::json::object();

    if(AllOf<Arcadia::CameraComponent>(name))
    {
        json_comps.push_back({ Arcadia::CameraComponent::GetTypeStrStatic(),Get<Arcadia::CameraComponent>(name).ToJson() });
    }
    if(AllOf<Arcadia::LightComponent>(name))
    {
        json_comps.push_back({ Arcadia::LightComponent::GetTypeStrStatic(),Get<Arcadia::LightComponent>(name).ToJson() });
    }
    if(AllOf<Arcadia::ModelComponent>(name))
    {
        json_comps.push_back({ Arcadia::ModelComponent::GetTypeStrStatic(),Get<Arcadia::ModelComponent>(name).ToJson() });
    }
    if(AllOf<Arcadia::PhysicsComponent>(name))
    {
        json_comps.push_back({ Arcadia::PhysicsComponent::GetTypeStrStatic(),Get<Arcadia::PhysicsComponent>(name).ToJson() });
    }
    if(AllOf<Arcadia::TransformComponent>(name))
    {
        json_comps.push_back({ Arcadia::TransformComponent::GetTypeStrStatic(), Get<Arcadia::TransformComponent>(name).ToJson() });
    }

    return json_comps;
}

auto Arcadia::EntityInfo::GetName() const -> const std::string&
{
    return Name;
}

auto Arcadia::EntityInfo::GetEntity() const -> entt::entity
{
    return Entity;
}
