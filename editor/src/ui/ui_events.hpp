#pragma once

#include<memory>
#include<string>

#include"core/event/event.hpp"

namespace Arcadia
{
    namespace Events
    {
        //==== Events for imgui window ====//
        ACDA_DEFINE_EVENT(
            OpenImguiWindow,
            std::string // Title of the imgui window to open
        );

        ACDA_DEFINE_EVENT(
            ScaleImguiWindow,
            float // New scale
        );

        //==== Events for project ====//

        ACDA_DEFINE_EVENT(
            NewProject
        );
        ACDA_DEFINE_EVENT(
            CreateProject,
            std::string, // name
            std::string  // filepath
        );
        ACDA_DEFINE_EVENT(
            OpenProject
        );
        ACDA_DEFINE_EVENT(
            SaveProject
        );
        ACDA_DEFINE_EVENT(
            SaveProjectAs
        );
        ACDA_DEFINE_EVENT(
            CloseProject
        );

        //==== Events for physics simulator ====//

        ACDA_DEFINE_EVENT(
            PhysicsSimulatorShouldUpdate,
            bool
        );

        //==== Events for scene ====//

        ACDA_DEFINE_EVENT(
            NewScene
        );
        ACDA_DEFINE_EVENT(
            CreateScene,
            std::string, // name
            bool        // as_current
        );
        ACDA_DEFINE_EVENT(
            SelectScene,
            std::string // name
        );
        ACDA_DEFINE_EVENT(
            CloseScene
        );
        ACDA_DEFINE_EVENT(
            DeleteScene
        );

        //==== Events for entity ====//

        ACDA_DEFINE_EVENT(
            NewEntity,
            std::string // type
        );
        ACDA_DEFINE_EVENT(
            SelectEntity,
            std::string // entity name
        );
        ACDA_DEFINE_EVENT(
            RenameEntity,
            std::string, // old name
            std::string  // new name
        );
        ACDA_DEFINE_EVENT(
            DeleteEntity,
            std::string // entity name
        );

        //==== Events for component ====//

        ACDA_DEFINE_EVENT(
            AddComponent,
            std::string, // entity name
            std::string // type_str
        );
        ACDA_DEFINE_EVENT(
            RemoveComponent,
            std::string, // entity name
            std::string // type_str
        );

        //==== Events for physics component ====//

        ACDA_DEFINE_EVENT(
            PhysicsComponentNewBody
        );

        //==== Events for modes ====//

        ACDA_DEFINE_EVENT(
            TogglePlayMode,
            bool
        );

        //==== Events for viewport ====//

        ACDA_DEFINE_EVENT(
            ShowGizmo,
            bool
        );
    }
}