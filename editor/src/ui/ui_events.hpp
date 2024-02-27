#pragma once

#include<string>

#include"core/event/event.hpp"

namespace arcadia
{
    namespace event
    {
        ARCADIA_EVENT(
            new_project
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

        ARCADIA_EVENT(
            new_scene
        );
        ARCADIA_EVENT(
            select_scene,
            std::string
        );
        ARCADIA_EVENT(
            delete_scene
        );
    }
}
