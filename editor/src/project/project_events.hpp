#pragma once

#include<memory>

#include"core/event/event.hpp"

namespace arcadia
{
    struct project;
    struct renderer_interface;
    struct scene;

    namespace event
    {

        //==== Event for project ====//

        ARCADIA_EVENT(
            project_built,
            std::weak_ptr<arcadia::project>
        );
        ARCADIA_EVENT(
            project_unbuilt
        );

        //==== Event for renderer ====//

        ARCADIA_EVENT(
            renderer_built,
            std::weak_ptr<arcadia::renderer_interface>
        );
        ARCADIA_EVENT(
            renderer_unbuilt
        );

        //==== Event for scene ====//

        ARCADIA_EVENT(
            scene_built,
            std::weak_ptr<arcadia::scene>
        );
        ARCADIA_EVENT(
            scene_activated,
            std::weak_ptr<arcadia::scene>
        );
        ARCADIA_EVENT(
            scene_deactivated
        );
        ARCADIA_EVENT(
            scene_unbuilt
        );
    }
}
