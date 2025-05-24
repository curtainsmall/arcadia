#pragma once

#include <functional>
#include <memory>

#include "core/nlohmann_json.hpp"

#include "core/event.hpp"
#include "resource/scene.hpp"

namespace Arcadia::Events
{
    struct UpdateEntityInfo: public EventBase
    {
    public:
        using EntityInfoModifierType = std::function<void(EntityInfo&)>;
    public:
        UpdateEntityInfo(EntityId entity_id, const EntityInfoModifierType& entity_info_modifier):
            EntityId(entity_id),
            EntityInfoModifier(entity_info_modifier)
        {
        }
    public:
        EntityId EntityId;
        const EntityInfoModifierType EntityInfoModifier;
    };

    //==== Events for scene ====//

    struct SceneBuilt: public EventBase
    {
    public:
        SceneBuilt(const std::shared_ptr<Scene>& scene_sptr):
            spScene(scene_sptr)
        {
        }
    public:
        const std::shared_ptr<Scene> spScene;
    };

    struct SceneActivated: public EventBase
    {
    public:
        SceneActivated(const std::shared_ptr<Scene>& scene_sptr):
            spScene(scene_sptr)
        {
        }
    public:
        const std::shared_ptr<Scene> spScene;
    };

    struct SceneDeactivated: public EventBase
    {
    };

    struct SceneUnbuilt: public EventBase
    {
    };

    struct NewScene: public EventBase
    {
    };

    struct CreateScene: public EventBase
    {
    public:
        CreateScene(const std::string& name, bool as_current):
            Name(name), AsCurrent(as_current)
        {
        }
    public:
        const std::string Name;
        const bool AsCurrent;
    };

    struct CreateSceneFromJson: public EventBase
    {
    public:
        CreateSceneFromJson(const nlohmann::json& json):
            Json(json)
        {
        }
    public:
        const nlohmann::json Json;
    };

    struct RenameScene: public EventBase
    {
    public:
        RenameScene(const std::string& new_name):
            NewName(new_name)
        {
        }
    public:
        const std::string NewName;
    };

    struct SelectScene: public EventBase
    {
    public:
        SelectScene(const std::string& name):
            Name(name)
        {
        }
    public:
        const std::string Name;
    };

    struct CloseScene: public EventBase
    {
    };

    struct DeleteScene: public EventBase
    {
    };

    struct DestroyAllScenes: public EventBase
    {
    };

    //==== Events for entity ====//

    struct NewEntity: public EventBase
    {
    public:
        NewEntity(const std::string& entity_type_string):
            EntityTypeString(entity_type_string)
        {
        }
    public:
        const std::string EntityTypeString;
    };

    struct SelectEntity: public EventBase
    {
    public:
        SelectEntity(EntityId entity_id):
            EntityId(entity_id)
        {
        }
    public:
        const EntityId EntityId;
    };

    struct RenameEntity: public EventBase
    {
    public:
        RenameEntity(EntityId entity_id, const std::string& new_name):
            EntityId(entity_id), NewName(new_name)
        {
        }
    public:
        const EntityId EntityId;
        const std::string NewName;
    };

    struct DeleteEntity: public EventBase
    {
    public:
        DeleteEntity(EntityId entity_id):
            EntityId(entity_id)
        {
        }
    public:
        const EntityId EntityId;
    };

    //==== Events for component ====//

    struct AddComponent: public EventBase
    {
    public:
        AddComponent(EntityId entity_id, const std::string& component_type_string):
            EntityId(entity_id), ComponentTypeString(component_type_string)
        {
        }
    public:
        const EntityId EntityId;
        const std::string ComponentTypeString;
    };

    struct RemoveComponent: public EventBase
    {
    public:
        RemoveComponent(EntityId entity_id, const std::string& component_type_string):
            EntityId(entity_id), ComponentTypeString(component_type_string)
        {
        }
    public:
        const EntityId EntityId;
        const std::string ComponentTypeString;
    };
}