#pragma once

#include<memory>
#include<string>

#include"core/event/event.hpp"
#include"resource/scene/entt_header.hpp"

namespace Arcadia
{
    namespace Event
    {
        //==== Events for imgui window ====//
        ARCADIA_EVENT(
            OpenImguiWindow,
            std::string // Title of the imgui window to open
        );

        //==== Events for project ====//

        ARCADIA_EVENT(
            NewProject
        );
        ARCADIA_EVENT(
            CreateProject,
            std::string, // name
            std::string  // filepath
        );
        ARCADIA_EVENT(
            OpenProject
        );
        ARCADIA_EVENT(
            SaveProject
        );
        ARCADIA_EVENT(
            SaveProjectAs
        );
        ARCADIA_EVENT(
            CloseProject
        );

        //==== Events for physics simulator ====//

        ARCADIA_EVENT(
            PhysicsSimulatorShouldUpdate,
            bool
        );

        //==== Events for scene ====//

        ARCADIA_EVENT(
            NewScene
        );
        ARCADIA_EVENT(
            CreateScene,
            std::string, // name
            bool        // as_current
        );
        ARCADIA_EVENT(
            SelectScene,
            std::string // name
        );
        ARCADIA_EVENT(
            CloseScene
        );
        ARCADIA_EVENT(
            DeleteScene
        );

        //==== Events for entity ====//

        ARCADIA_EVENT(
            NewEntity
        );
        ARCADIA_EVENT(
            SelectEntity,
            entt::entity // entity
        );
        ARCADIA_EVENT(
            DeleteEntity,
            entt::entity // entity
        );

        //==== Events for component ====//

        ARCADIA_EVENT(
            AddComponent,
            entt::entity, // entity
            std::string // type_str
        );
        ARCADIA_EVENT(
            RemoveComponent,
            entt::entity, // entity
            std::string // type_str
        );

        //==== Events for physics component ====//

        ARCADIA_EVENT(
            PhysicsComponentNewBody
        );
    }
}
