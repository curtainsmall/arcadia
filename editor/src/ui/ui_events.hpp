#pragma once

#include <memory>
#include <string>

#include "core/event/event.hpp"
#include "resource/entity_id.hpp"

namespace Arcadia
{
    namespace Events
    {
        //==== Events for imgui window ====//
        class OpenImguiWindow: public EventBase
        {
        public:
            OpenImguiWindow(const std::string& id_string):
                IdString(id_string)
            {}
        public:
            const std::string IdString;
        };

        class ScaleImguiWindow: public EventBase
        {
        public:
            ScaleImguiWindow(float new_scale):
                NewScale(new_scale)
            {}
        public:
            const float NewScale;
        };

        //==== Events for project ====//

        class NewProject: public EventBase
        {};

        class CreateProject: public EventBase
        {
        public:
            CreateProject(const std::string& name, const std::string& filepath_string):
                Name(name), FilepathString(filepath_string)
            {}
        public:
            const std::string Name;
            const std::string FilepathString;
        };

        class OpenProject: public EventBase
        {};

        class SaveProject: public EventBase
        {};

        class SaveProjectAs: public EventBase
        {};

        class CloseProject: public EventBase
        {};

        //==== Events for physics simulator ====//

        class PhysicsSimulatorShouldUpdate: public EventBase
        {
        public:
            PhysicsSimulatorShouldUpdate(bool should_update):
                ShouldUpdate(should_update)
            {}
        public:
            const bool ShouldUpdate;
        };

        //==== Events for scene ====//

        class NewScene: public EventBase
        {};

        class CreateScene: public EventBase
        {
        public:
            CreateScene(const std::string& name, bool as_current):
                Name(name), AsCurrent(as_current)
            {}
        public:
            const std::string Name;
            const bool AsCurrent;
        };

        class RenameScene: public EventBase
        {
        public:
            RenameScene(const std::string& new_name):
                NewName(new_name)
            {}
        public:
            const std::string NewName;
        };

        class SelectScene: public EventBase
        {
        public:
            SelectScene(const std::string& name):
                Name(name)
            {}
        public:
            const std::string Name;
        };

        class CloseScene: public EventBase
        {};

        class DeleteScene: public EventBase
        {};

        //==== Events for entity ====//

        class NewEntity: public EventBase
        {
        public:
            NewEntity(const std::string& entity_type_string):
                EntityTypeString(entity_type_string)
            {}
        public:
            const std::string EntityTypeString;
        };

        class SelectEntity: public EventBase
        {
        public:
            SelectEntity(EntityId entity_id):
                EntityId(entity_id)
            {}
        public:
            const EntityId EntityId;
        };

        class RenameEntity: public EventBase
        {
        public:
            RenameEntity(EntityId entity_id, const std::string& new_name):
                EntityId(entity_id), NewName(new_name)
            {}
        public:
            const EntityId EntityId;
            const std::string NewName;
        };

        class DeleteEntity: public EventBase
        {
        public:
            DeleteEntity(EntityId entity_id):
                EntityId(entity_id)
            {}
        public:
            const EntityId EntityId;
        };

        //==== Events for component ====//

        class AddComponent: public EventBase
        {
        public:
            AddComponent(EntityId entity_id, const std::string& component_type_string):
                EntityId(entity_id), ComponentTypeString(component_type_string)
            {}
        public:
            const EntityId EntityId;
            const std::string ComponentTypeString;
        };

        class RemoveComponent: public EventBase
        {
        public:
            RemoveComponent(EntityId entity_id, const std::string& component_type_string):
                EntityId(entity_id), ComponentTypeString(component_type_string)
            {}
        public:
            const EntityId EntityId;
            const std::string ComponentTypeString;
        };

        //==== Events for physics component ====//

        class PhysicsComponentNewBody: public EventBase
        {};

        //==== Events for modes ====//

        class TogglePlayMode: public EventBase
        {};

        //==== Events for viewport ====//

        class ShowGizmo: public EventBase
        {
        public:
            ShowGizmo(bool should_show_gizmo):
                ShouldShowGizmo(should_show_gizmo)
            {}
        public:
            const bool ShouldShowGizmo;
        };
    }
}