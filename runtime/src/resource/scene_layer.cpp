#include "pch.hpp"
#include "scene_layer.hpp"

#include "core/command.hpp"
#include "core/function.hpp"
#include "core/match.hpp"
#include "core/pfd.hpp"
#include "function/render/renderer_events.hpp"
#include "resource/components/camera_component.hpp"
#include "resource/components/light_component.hpp"
#include "resource/components/model_component.hpp"
#include "resource/components/physics_component.hpp"
#include "resource/components/transform_component.hpp"

Arcadia::SceneLayer::SceneLayer():
    LayerInterface("scene")
{
}

Arcadia::SceneLayer::~SceneLayer()
{
}

void Arcadia::SceneLayer::OnEvent(EventBase& event)
{
    EventDispatcher(event)
        .Dispatch<Events::CreateScene>(ACDA_BIND_MEMBER_FN(_OnCreateScene))
        .Dispatch<Events::RenameScene>(ACDA_BIND_MEMBER_FN(_OnRenameScene))
        .Dispatch<Events::SelectScene>(ACDA_BIND_MEMBER_FN(_OnSelectScene))
        .Dispatch<Events::CloseScene>(ACDA_BIND_MEMBER_FN(_OnCloseScene))
        .Dispatch<Events::DeleteScene>(ACDA_BIND_MEMBER_FN(_OnDeleteScene))
        .Dispatch<Events::NewEntity>(ACDA_BIND_MEMBER_FN(_OnNewEntity))
        .Dispatch<Events::RenameEntity>(ACDA_BIND_MEMBER_FN(_OnRenameEntity))
        .Dispatch<Events::DeleteEntity>(ACDA_BIND_MEMBER_FN(_OnDeleteEntity))
        .Dispatch<Events::UpdateEntityInfo>(ACDA_BIND_MEMBER_FN(_OnUpdateEntityInfo))
        .Dispatch<Events::AddComponent>(ACDA_BIND_MEMBER_FN(_OnAddComponent))
        .Dispatch<Events::RemoveComponent>(ACDA_BIND_MEMBER_FN(_OnRemoveComponent))
        .IsDispatched();
}

void Arcadia::SceneLayer::OnUpdate()
{
    if(_spActiveScene)
    {
        for(auto& [entity_id, entity_info] : _spActiveScene->GetEntityInfoStorage())
        {
            if(entity_info.Displayed)
            {
                EventQueue::Instance()
                    .Signal<Events::RendererSetEntity>(
                        entity_id,
                        Events::RendererSetEntity_ActionType::Add
                    );
            }
            else
            {
                EventQueue::Instance()
                    .Signal<Events::RendererSetEntity>(
                        entity_id,
                        Events::RendererSetEntity_ActionType::Remove
                    );
            }
        }
    }
}

auto Arcadia::SceneLayer::GetActiveSceneShared() -> std::shared_ptr<Scene>&
{
    ACDA_ASSERT(HasActiveScene());

    return _spActiveScene;
}

auto Arcadia::SceneLayer::GetActiveSceneShared() const -> const std::shared_ptr<Scene>&
{
    ACDA_ASSERT(HasActiveScene());

    return _spActiveScene;
}

void Arcadia::SceneLayer::SetActiveScene(const std::string& name)
{
    bool is_same_scene = HasActiveScene() && name == _spActiveScene->GetName();

    if(!is_same_scene)
    {
        CommandList::Instance().Clear();

        if(HasActiveScene())
        {
            _spActiveScene.reset();
            EventQueue::Instance()
                .Signal<Events::SceneDeactivated>();
        }

        if(!name.empty() && _SceneStorage.contains(name))
        {
            _spActiveScene = _SceneStorage.at(name);

            EventQueue::Instance()
                .Signal<Events::SceneActivated>(_spActiveScene);
        }
    }
}

auto Arcadia::SceneLayer::HasScene() const -> bool
{
    return !_SceneStorage.empty();
}

auto Arcadia::SceneLayer::HasScene(const std::string& name) const -> bool
{
    return _SceneStorage.contains(name);
}

auto Arcadia::SceneLayer::HasActiveScene() const -> bool
{
    return !!_spActiveScene;
}

void Arcadia::SceneLayer::CreateScene(const std::string& name)
{
    ACDA_ASSERT(!HasScene(name));

    _SceneStorage.try_emplace(
        name,
        std::make_shared<Scene>(name)
    );
}

void Arcadia::SceneLayer::CreateScene(const nlohmann::json& json)
{
    try
    {
        _SceneStorage.try_emplace(
            json.at("name"),
            std::make_shared<Scene>(json)
        );
    }
    catch(const nlohmann::json::exception& e)
    {
        throw Exceptions::CreatingSceneFromJsonFailed(std::format("Json failure: {}", e.what()));
    }
}

auto Arcadia::SceneLayer::SaveScene(const std::string& name) -> nlohmann::json
{
    nlohmann::json json = nlohmann::json::array();
    json.push_back(_SceneStorage.at(name)->ToJson());
    return json;
}

void Arcadia::SceneLayer::DestroyScene(const std::string& name)
{
    ACDA_ASSERT(HasScene(name));

    _SceneStorage.erase(name);
    if(name == GetActiveSceneShared()->GetName())
    {
        SetActiveScene();
    }
}

void Arcadia::SceneLayer::DestroyAllScenes()
{
    _SceneStorage.clear();
    SetActiveScene();
}

void Arcadia::SceneLayer::RenameScene(const std::string& name, const std::string& new_name)
{
    ACDA_ASSERT(HasScene(name));
    ACDA_ASSERT(!HasScene(new_name));

    SceneStorageType::node_type node = _SceneStorage.extract(name);
    node.key() = new_name;
    node.mapped()->SetName(new_name);
    _SceneStorage.insert(std::move(node));
}

auto Arcadia::SceneLayer::GetSceneStorage() const -> const SceneStorageType&
{
    return _SceneStorage;
}

auto Arcadia::SceneLayer::ActiveScene_GetName() const -> const std::string&
{
    return _spActiveScene->GetName();
}

auto Arcadia::SceneLayer::ActiveScene_ContainsEntity(EntityId entity_id) const -> bool
{
    return _spActiveScene->ContainsEntity(entity_id);
}

auto Arcadia::SceneLayer::ActiveScene_IsEntityNameUsed(const std::string& entity_name) const -> bool
{
    return _spActiveScene->IsEntityNameUsed(entity_name);
}

auto Arcadia::SceneLayer::ActiveScene_GetEntityIdByName(const std::string& entity_name) const -> EntityId
{
    return _spActiveScene->GetEntityIdByName(entity_name);
}

auto Arcadia::SceneLayer::ActiveScene_GetEntityCount() const -> std::size_t
{
    return _spActiveScene->GetEntityCount();
}

auto Arcadia::SceneLayer::ActiveScene_GetEntityCount(const std::function<bool(EntityId, const EntityInfo&)>& pred) const -> std::size_t
{
    return _spActiveScene->GetEntityCount(pred);
}

auto Arcadia::SceneLayer::ActiveScene_GetEntityInfo(EntityId entity_id) const -> const EntityInfo&
{
    return _spActiveScene->GetEntityInfo(entity_id);
}

auto Arcadia::SceneLayer::ActiveScene_GetEntityInfoStorage() const -> const Scene::EntityInfoStorageType&
{
    return _spActiveScene->GetEntityInfoStorage();
}

auto Arcadia::SceneLayer::IsActiveSceneModified() const -> bool
{
    return _ActiveSceneModified;
}

void Arcadia::SceneLayer::_OnCreateScene(Events::CreateScene& e)
{
    CreateScene(e.Name);
    if(e.AsCurrent)
    {
        SetActiveScene(e.Name);
    }
    _ActiveSceneModified = true;
}

void Arcadia::SceneLayer::_OnRenameScene(Events::RenameScene& e)
{
    RenameScene(GetActiveSceneShared()->GetName(), e.NewName);
    _ActiveSceneModified = true;
}

void Arcadia::SceneLayer::_OnSelectScene(Events::SelectScene& e)
{
    SetActiveScene(e.Name);
    _ActiveSceneModified = true;
}

void Arcadia::SceneLayer::_OnCloseScene(Events::CloseScene& e)
{
    SetActiveScene();
    _ActiveSceneModified = true;
}

void Arcadia::SceneLayer::_OnDeleteScene(Events::DeleteScene& e)
{
    const std::string& scene_name = GetActiveSceneShared()->GetName();

    pfd::button res = pfd::message(
        "Arcadia - Delete Scene",
        std::format("Do you want to delet scene: {}", scene_name)
    ).result();

    switch(res)
    {
        case pfd::button::ok:
        {
            DestroyScene(scene_name);
            break;
        }
        case pfd::button::cancel:
        default:
        {
            break;
        }
    }
    _ActiveSceneModified = true;
}

void Arcadia::SceneLayer::_OnNewEntity(Events::NewEntity& e)
{
    Scene& scene = *GetActiveSceneShared();

    std::string temp_name = "New Entity";
    std::string name = temp_name;
    std::int32_t postfix{ 1 };
    while(scene.IsEntityNameUsed(name))
    {
        name = std::format("{} {}", temp_name, ++postfix);
    }

    EntityId entity_id = scene.CreateEntity(name, e.EntityTypeString);

    Match<void>(
        e.EntityTypeString,
        std::string("actor"),
        [&]()
    {
        scene.EmplaceComponent<ModelComponent>(entity_id);

        scene.EmplaceComponent<PhysicsComponent>(entity_id);

        TransformComponent& transform_comp = scene.EmplaceComponent<TransformComponent>(entity_id);
        transform_comp.AddFlag(TransformComponentFlags::UseRotation);
    },
        std::string("camera"),
        [&]()
    {
        scene.EmplaceComponent<CameraComponent>(entity_id);

        TransformComponent& transform_comp = scene.EmplaceComponent<TransformComponent>(entity_id);
        transform_comp.AddFlag(TransformComponentFlags::UseDirection);
    },
        std::string("light"),
        [&]()
    {
        scene.EmplaceComponent<LightComponent>(entity_id);

        TransformComponent& transform_comp = scene.EmplaceComponent<TransformComponent>(entity_id);
        transform_comp.AddFlag(TransformComponentFlags::UseDirection);
    }
    );
    _ActiveSceneModified = true;
}

void Arcadia::SceneLayer::_OnRenameEntity(Events::RenameEntity& e)
{
    GetActiveSceneShared()->RenameEntity(e.EntityId, e.NewName);
    _ActiveSceneModified = true;
}

void Arcadia::SceneLayer::_OnDeleteEntity(Events::DeleteEntity& e)
{
    GetActiveSceneShared()->DestroyEntity(e.EntityId);
    _ActiveSceneModified = true;
}

void Arcadia::SceneLayer::_OnUpdateEntityInfo(Events::UpdateEntityInfo& e)
{
    e.EntityInfoModifier(_spActiveScene->GetEntityInfo(e.EntityId));
}

void Arcadia::SceneLayer::_OnAddComponent(Events::AddComponent& e)
{
    Scene& scene = *GetActiveSceneShared();

    Match<void>(
        e.ComponentTypeString,
        CameraComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.EmplaceComponent<CameraComponent>(e.EntityId);
    },
        LightComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.EmplaceComponent<LightComponent>(e.EntityId);
    },
        ModelComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.EmplaceComponent<ModelComponent>(e.EntityId);
    },
        PhysicsComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.EmplaceComponent<PhysicsComponent>(e.EntityId);
    }
    );
    _ActiveSceneModified = true;
}

void Arcadia::SceneLayer::_OnRemoveComponent(Events::RemoveComponent& e)
{
    Scene& scene = *GetActiveSceneShared();

    Match<void>(
        e.ComponentTypeString,
        CameraComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.RemoveComponent<CameraComponent>(e.EntityId);
    },
        LightComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.RemoveComponent<LightComponent>(e.EntityId);
    },
        ModelComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.RemoveComponent<ModelComponent>(e.EntityId);
    },
        PhysicsComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.RemoveComponent<PhysicsComponent>(e.EntityId);
    }
    );
    _ActiveSceneModified = true;
}