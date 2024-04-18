#pragma once

#include<memory>
#include<string>

#include"core/event/event.hpp"

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
            NewEntity,
            std::string // type
        );
        ARCADIA_EVENT(
            SelectEntity,
            std::string // entity name
        );
        ARCADIA_EVENT(
            DeleteEntity,
            std::string // entity name
        );

        //==== Events for component ====//

        ARCADIA_EVENT(
            AddComponent,
            std::string, // entity name
            std::string // type_str
        );
        ARCADIA_EVENT(
            RemoveComponent,
            std::string, // entity name
            std::string // type_str
        );

        //==== Events for physics component ====//

        ARCADIA_EVENT(
            PhysicsComponentNewBody
        );
    }
}
