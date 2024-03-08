#pragma once

#include<memory>
#include<string>

#include"core/event/event.hpp"
#include"resource/scene/entt_header.hpp"

namespace arcadia
{
    namespace event
    {
        //==== Events for imgui window ====//
        ARCADIA_EVENT(
            open_imgui_window,
            std::string // Title of the imgui window to open
        );

        //==== Events for project ====//

        // used to tell imgui to popup a window for inputting project info
        ARCADIA_EVENT(
            new_project
        );
        // used to tell project layer to create project with inputted info
        ARCADIA_EVENT(
            create_project,
            std::string, // name
            std::string  // filepath
        );
        ARCADIA_EVENT(
            open_project
        );
        ARCADIA_EVENT(
            save_project
        );
        ARCADIA_EVENT(
            save_project_as
        );
        ARCADIA_EVENT(
            close_project
        );

        //==== Events for scene ====//

        // used to tell imgui to popup a window for inputting scene info
        ARCADIA_EVENT(
            new_scene
        );
        // used to tell project layer to create scene with inputted info
        ARCADIA_EVENT(
            create_scene,
            std::string, // name
            bool        // as_current
        );
        ARCADIA_EVENT(
            select_scene,
            std::string // name
        );
        ARCADIA_EVENT(
            close_scene
        );
        ARCADIA_EVENT(
            delete_scene
        );

        //==== Events for entity ====//

        ARCADIA_EVENT(
            new_entity
        );
        ARCADIA_EVENT(
            select_entity,
            entt::entity // entity
        );
        ARCADIA_EVENT(
            rename_entity,
            std::string, // old_name
            std::string  // new_name
        );
        ARCADIA_EVENT(
            delete_entity,
            entt::entity // entity
        );

        //==== Events for component ====//

        ARCADIA_EVENT(
            add_component,
            entt::entity, // entity
            std::string // type_str
        );
        ARCADIA_EVENT(
            remove_component,
            entt::entity, // entity
            std::string // type_str
        );
    }
}
