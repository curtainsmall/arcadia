#pragma once

#include <functional>
#include <memory>

#include "core/nlohmann_json.hpp"

#include "core/event.hpp"
#include "resource/scene.hpp"

namespace Arcadia::Events
{
    struct ACDA_API UpdateEntityInfo: public EventBase
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

    struct ACDA_API SceneBuilt: public EventBase
    {
    public:
        SceneBuilt(const std::shared_ptr<Scene>& scene_sptr):
            spScene(scene_sptr)
        {
        }
    public:
        const std::shared_ptr<Scene> spScene;
    };

    struct ACDA_API SceneActivated: public EventBase
    {
    public:
        SceneActivated(const std::shared_ptr<Scene>& scene_sptr):
            spScene(scene_sptr)
        {
        }
    public:
        const std::shared_ptr<Scene> spScene;
    };

    struct ACDA_API SceneDeactivated: public EventBase
    {
    };

    struct ACDA_API SceneUnbuilt: public EventBase
    {
    };

    struct ACDA_API NewScene: public EventBase
    {
    };

    struct ACDA_API CreateScene: public EventBase
    {
    public:
        CreateScene(std::string_view name, bool as_current):
            Name(name), AsCurrent(as_current)
        {
        }
    public:
        const std::string Name;
        const bool AsCurrent;
    };

    struct ACDA_API CreateSceneFromJson: public EventBase
    {
    public:
        CreateSceneFromJson(const nlohmann::json& json):
            Json(json)
        {
        }
    public:
        const nlohmann::json Json;
    };

    struct ACDA_API RenameScene: public EventBase
    {
    public:
        RenameScene(std::string_view new_name):
            NewName(new_name)
        {
        }
    public:
        const std::string NewName;
    };

    struct ACDA_API SelectScene: public EventBase
    {
    public:
        SelectScene(std::string_view name):
            Name(name)
        {
        }
    public:
        const std::string Name;
    };

    struct ACDA_API CloseScene: public EventBase
    {
    };

    struct ACDA_API DeleteScene: public EventBase
    {
    };

    struct ACDA_API DestroyAllScenes: public EventBase
    {
    };

    //==== Events for entity ====//

    struct ACDA_API NewEntity: public EventBase
    {
    public:
        NewEntity(std::string_view entity_type_string):
            EntityTypeString(entity_type_string)
        {
        }
    public:
        const std::string EntityTypeString;
    };

    struct ACDA_API SelectEntity: public EventBase
    {
    public:
        SelectEntity(EntityId entity_id):
            EntityId(entity_id)
        {
        }
    public:
        const EntityId EntityId;
    };

    struct ACDA_API RenameEntity: public EventBase
    {
    public:
        RenameEntity(EntityId entity_id, std::string_view new_name):
            EntityId(entity_id), NewName(new_name)
        {
        }
    public:
        const EntityId EntityId;
        const std::string NewName;
    };

    struct ACDA_API DeleteEntity: public EventBase
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

    struct ACDA_API AddComponent: public EventBase
    {
    public:
        AddComponent(EntityId entity_id, std::string_view component_type_string):
            EntityId(entity_id), ComponentTypeString(component_type_string)
        {
        }
    public:
        const EntityId EntityId;
        const std::string ComponentTypeString;
    };

    struct ACDA_API RemoveComponent: public EventBase
    {
    public:
        RemoveComponent(EntityId entity_id, std::string_view component_type_string):
            EntityId(entity_id), ComponentTypeString(component_type_string)
        {
        }
    public:
        const EntityId EntityId;
        const std::string ComponentTypeString;
    };
}