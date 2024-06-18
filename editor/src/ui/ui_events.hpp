#pragma once

#include<memory>
#include<string>

#include"core/event/event.hpp"

namespace Event
{
    //==== Events for imgui window ====//
    ACDA_EVENT(
        OpenImguiWindow,
        std::string // Title of the imgui window to open
    );

    //==== Events for project ====//

    ACDA_EVENT(
        NewProject
    );
    ACDA_EVENT(
        CreateProject,
        std::string, // name
        std::string  // filepath
    );
    ACDA_EVENT(
        OpenProject
    );
    ACDA_EVENT(
        SaveProject
    );
    ACDA_EVENT(
        SaveProjectAs
    );
    ACDA_EVENT(
        CloseProject
    );

    //==== Events for physics simulator ====//

    ACDA_EVENT(
        PhysicsSimulatorShouldUpdate,
        bool
    );

    //==== Events for scene ====//

    ACDA_EVENT(
        NewScene
    );
    ACDA_EVENT(
        CreateScene,
        std::string, // name
        bool        // as_current
    );
    ACDA_EVENT(
        SelectScene,
        std::string // name
    );
    ACDA_EVENT(
        CloseScene
    );
    ACDA_EVENT(
        DeleteScene
    );

    //==== Events for entity ====//

    ACDA_EVENT(
        NewEntity,
        std::string // type
    );
    ACDA_EVENT(
        SelectEntity,
        std::string // entity name
    );
    ACDA_EVENT(
        RenameEntity,
        std::string, // old name
        std::string  // new name
    );
    ACDA_EVENT(
        DeleteEntity,
        std::string // entity name
    );

    //==== Events for component ====//

    ACDA_EVENT(
        AddComponent,
        std::string, // entity name
        std::string // type_str
    );
    ACDA_EVENT(
        RemoveComponent,
        std::string, // entity name
        std::string // type_str
    );

    //==== Events for physics component ====//

    ACDA_EVENT(
        PhysicsComponentNewBody
    );

    //==== Events for modes ====//

    ACDA_EVENT(
        PlayMode,
        bool
    );

    //==== Events for viewport ====//

    ACDA_EVENT(
        ShowGizmo,
        bool
    );
}
