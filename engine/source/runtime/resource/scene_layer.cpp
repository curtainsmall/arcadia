#include "scene_layer.hpp"

#include "core/command.hpp"
#include "core/function.hpp"
#include "core/match.hpp"
#include "core/pfd.hpp"
#include "function/physics/physics_layer.hpp"
#include "function/render/renderer_events.hpp"
#include "function/render/renderer_layer.hpp"
#include "resource/components.hpp"

Arcadia::SceneLayer::SceneLayer():
    LayerInterface("scene")
{
}

void Arcadia::SceneLayer::OnEvent(EventBase& event)
{
    EventDispatcher(event)
        .Dispatch<Events::CreateScene>(ACDA_BIND_MEMBER_FN(_OnCreateScene))
        .Dispatch<Events::CreateSceneFromJson>(ACDA_BIND_MEMBER_FN(_OnCreateSceneFromJson))
        .Dispatch<Events::RenameScene>(ACDA_BIND_MEMBER_FN(_OnRenameScene))
        .Dispatch<Events::SelectScene>(ACDA_BIND_MEMBER_FN(_OnSelectScene))
        .Dispatch<Events::CloseScene>(ACDA_BIND_MEMBER_FN(_OnCloseScene))
        .Dispatch<Events::DeleteScene>(ACDA_BIND_MEMBER_FN(_OnDeleteScene))
        .Dispatch<Events::DestroyAllScenes>(ACDA_BIND_MEMBER_FN(_OnDestroyAllScene))
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
                        Events::RendererSetEntity::ActionType::Build
                    );
            }
            else
            {
                EventQueue::Instance()
                    .Signal<Events::RendererSetEntity>(
                        entity_id,
                        Events::RendererSetEntity::ActionType::Remove
                    );
            }

            if(_spActiveScene->ContainsAllComponents<PhysicsComponent>(entity_id))
            {
                const PhysicsComponent& physics_comp = _spActiveScene->GetComponent<PhysicsComponent>(entity_id);
                if(physics_comp.IsValid())
                {
                    EventQueue::Instance()
                        .Signal<Events::PhysicsSimulatorSetEntity>(
                            entity_id,
                            Events::PhysicsSimulatorSetEntity::ActionType::Build
                        );
                }
                else
                {
                    EventQueue::Instance()
                        .Signal<Events::PhysicsSimulatorSetEntity>(
                            entity_id,
                            Events::PhysicsSimulatorSetEntity::ActionType::Remove
                        );
                }
            }
        }
    }
}

auto Arcadia::SceneLayer::GetActiveSceneShared() const -> const std::shared_ptr<Scene>&
{
    ACDA_ASSERT(HasActiveScene());

    return _spActiveScene;
}

void Arcadia::SceneLayer::_SetActiveScene(std::string_view name, bool not_considered_modified)
{
    if(HasActiveScene() && name == _spActiveScene->GetName())
    {
        return;
    }

    CommandList::Instance().Clear();

    if(HasActiveScene())
    {
        _spActiveScene.reset();
        EventQueue::Instance()
            .Signal<Events::SceneDeactivated>();
    }

    if(!name.empty() && _SceneStorage.contains(std::string(name)))
    {
        _spActiveScene = _SceneStorage.at(std::string(name));

        EventQueue::Instance()
            .Signal<Events::SceneActivated>(_spActiveScene);
    }
    if(!not_considered_modified)
    {
        _ActiveSceneModificationFlag |= ActiveSceneModificationFlag::Self;
    }
}

auto Arcadia::SceneLayer::HasScene() const -> bool
{
    return !_SceneStorage.empty();
}

auto Arcadia::SceneLayer::HasScene(std::string_view name) const -> bool
{
    return _SceneStorage.contains(std::string(name));
}

auto Arcadia::SceneLayer::HasActiveScene() const -> bool
{
    return !!_spActiveScene;
}

void Arcadia::SceneLayer::_CreateScene(std::string_view name)
{
    ACDA_ASSERT(!HasScene(name));

    _SceneStorage.try_emplace(
        std::string(name),
        std::make_shared<Scene>(name)
    );
}

void Arcadia::SceneLayer::_CreateScene(const nlohmann::json& json)
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

auto Arcadia::SceneLayer::_SaveScene(std::string_view name) -> nlohmann::json
{
    nlohmann::json json = nlohmann::json::array();
    json.push_back(_SceneStorage.at(std::string(name))->ToJson());
    _ActiveSceneModificationFlag &= ~ActiveSceneModificationFlag::Content;
    return json;
}

void Arcadia::SceneLayer::_DestroyScene(std::string_view name)
{
    ACDA_ASSERT(HasScene(name));

    _SceneStorage.erase(std::string(name));
    if(name == GetActiveSceneShared()->GetName())
    {
        _SetActiveScene();
    }
}

void Arcadia::SceneLayer::_RenameScene(std::string_view name, std::string_view new_name)
{
    ACDA_ASSERT(HasScene(name));
    ACDA_ASSERT(!HasScene(new_name));

    SceneStorageType::node_type node = _SceneStorage.extract(std::string(name));
    node.key() = new_name;
    node.mapped()->SetName(new_name);
    _SceneStorage.insert(std::move(node));
}

auto Arcadia::SceneLayer::GetSceneStorage() const -> const SceneStorageType&
{
    return _SceneStorage;
}

auto Arcadia::SceneLayer::ActiveScene_GetName() const -> std::string_view
{
    ACDA_ASSERT(_spActiveScene);
    return _spActiveScene->GetName();
}

auto Arcadia::SceneLayer::ActiveScene_ContainsEntity(EntityId entity_id) const -> bool
{
    ACDA_ASSERT(_spActiveScene);
    return _spActiveScene->ContainsEntity(entity_id);
}

auto Arcadia::SceneLayer::ActiveScene_IsEntityNameUsed(std::string_view entity_name) const -> bool
{
    ACDA_ASSERT(_spActiveScene);
    return _spActiveScene->IsEntityNameUsed(entity_name);
}

auto Arcadia::SceneLayer::ActiveScene_GetEntityIdByName(std::string_view entity_name) const -> EntityId
{
    ACDA_ASSERT(_spActiveScene);
    return _spActiveScene->GetEntityIdByName(entity_name);
}

auto Arcadia::SceneLayer::ActiveScene_GetEntityCount() const -> std::size_t
{
    ACDA_ASSERT(_spActiveScene);
    return _spActiveScene->GetEntityCount();
}

auto Arcadia::SceneLayer::ActiveScene_GetEntityCount(const std::function<bool(EntityId, const EntityInfo&)>& pred) const -> std::size_t
{
    ACDA_ASSERT(_spActiveScene);
    return _spActiveScene->GetEntityCount(pred);
}

auto Arcadia::SceneLayer::ActiveScene_GetEntityInfo(EntityId entity_id) const -> const EntityInfo&
{
    ACDA_ASSERT(_spActiveScene);
    return _spActiveScene->GetEntityInfo(entity_id);
}

auto Arcadia::SceneLayer::ActiveScene_GetEntityInfoStorage() const -> const Scene::EntityInfoStorageType&
{
    ACDA_ASSERT(_spActiveScene);
    return _spActiveScene->GetEntityInfoStorage();
}

auto Arcadia::SceneLayer::IsActiveSceneModified(ActiveSceneModificationFlag type) const -> bool
{
    return !!(_ActiveSceneModificationFlag & type);
}

void Arcadia::SceneLayer::MarkActiveSceneModified(ActiveSceneModificationFlag type, bool modified)
{
    if(modified)
    {
        _ActiveSceneModificationFlag |= type;
    }
    else
    {
        _ActiveSceneModificationFlag &= ~type;
    }
}

void Arcadia::SceneLayer::Snapshot()
{
}

void Arcadia::SceneLayer::Restore()
{
}

void Arcadia::SceneLayer::_OnCreateScene(Events::CreateScene& e)
{
    _CreateScene(std::string_view(e.Name));
    if(e.AsCurrent)
    {
        _SetActiveScene(e.Name);
    }
}

void Arcadia::SceneLayer::_OnCreateSceneFromJson(Events::CreateSceneFromJson& e)
{
    _CreateScene(e.Json);
}

void Arcadia::SceneLayer::_OnRenameScene(Events::RenameScene& e)
{
    _RenameScene(GetActiveSceneShared()->GetName(), e.NewName);
    _ActiveSceneModificationFlag |= ActiveSceneModificationFlag::Name;
}

void Arcadia::SceneLayer::_OnSelectScene(Events::SelectScene& e)
{
    _SetActiveScene(e.Name, e.NotConsideredModified);
}

void Arcadia::SceneLayer::_OnCloseScene(Events::CloseScene& e)
{
    _SetActiveScene();
}

void Arcadia::SceneLayer::_OnDeleteScene(Events::DeleteScene& e)
{
    std::string_view scene_name = GetActiveSceneShared()->GetName();

    pfd::button res = pfd::message(
        "Arcadia - Delete Scene",
        std::format("Do you want to delet scene: {}", scene_name)
    ).result();

    switch(res)
    {
        case pfd::button::ok:
        {
            _DestroyScene(scene_name);
            break;
        }
        case pfd::button::cancel:
        default:
        {
            break;
        }
    }
}

void Arcadia::SceneLayer::_OnDestroyAllScene(Events::DestroyAllScenes& e)
{
    _SceneStorage.clear();
    _SetActiveScene();
}

void Arcadia::SceneLayer::_OnNewEntity(Events::NewEntity& e)
{
    Scene& scene = *GetActiveSceneShared();

    std::string_view temp_name = "New Entity";
    std::string name(temp_name);
    std::int32_t postfix{ 1 };

    // Append extra number for duplicated name
    while(scene.IsEntityNameUsed(name))
    {
        name = std::format("{} {}", temp_name, ++postfix);
    }

    EntityId entity_id = scene.CreateEntity(name, e.EntityTypeString);

    Match<void>(
        e.EntityTypeString,
        "actor",
        [&]()
        {
            scene.EmplaceComponent<ModelComponent>(entity_id);

            scene.EmplaceComponent<PhysicsComponent>(entity_id);

            TransformComponent& transform_comp = scene.EmplaceComponent<TransformComponent>(entity_id);
            transform_comp.AddFlag(TransformComponentFlags::UseRotation);

            scene.EmplaceComponent<ScriptComponent>(entity_id);
        },
        "camera",
        [&]()
        {
            scene.EmplaceComponent<CameraComponent>(entity_id);

            TransformComponent& transform_comp = scene.EmplaceComponent<TransformComponent>(entity_id);
            transform_comp.AddFlag(TransformComponentFlags::UseDirection);
        },
        "light",
        [&]()
        {
            scene.EmplaceComponent<LightComponent>(entity_id);

            TransformComponent& transform_comp = scene.EmplaceComponent<TransformComponent>(entity_id);
            transform_comp.AddFlag(TransformComponentFlags::UseDirection);
        }
    );
    _ActiveSceneModificationFlag |= ActiveSceneModificationFlag::Content;
}

void Arcadia::SceneLayer::_OnRenameEntity(Events::RenameEntity& e)
{
    GetActiveSceneShared()->RenameEntity(e.EntityId, e.NewName);
    _ActiveSceneModificationFlag |= ActiveSceneModificationFlag::Content;
}

void Arcadia::SceneLayer::_OnDeleteEntity(Events::DeleteEntity& e)
{
    GetActiveSceneShared()->DestroyEntity(e.EntityId);
    _ActiveSceneModificationFlag |= ActiveSceneModificationFlag::Content;
}

void Arcadia::SceneLayer::_OnUpdateEntityInfo(Events::UpdateEntityInfo& e)
{
    e.EntityInfoModifier(_spActiveScene->GetEntityInfo(e.EntityId));
}

void Arcadia::SceneLayer::_OnAddComponent(Events::AddComponent& e)
{
    Scene& scene = *GetActiveSceneShared();

    Match<void>(
        std::string_view(e.ComponentTypeString),
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
    _ActiveSceneModificationFlag |= ActiveSceneModificationFlag::Content;
}

void Arcadia::SceneLayer::_OnRemoveComponent(Events::RemoveComponent& e)
{
    Scene& scene = *GetActiveSceneShared();

    Match<void>(
        std::string_view(e.ComponentTypeString),
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
    _ActiveSceneModificationFlag |= ActiveSceneModificationFlag::Content;
}