#pragma once

#include<memory>

#include"core/event/event.hpp"
#include"resource/scene/scene.hpp"

namespace arcadia
{
    struct project;

    namespace event
    {
        ARCADIA_EVENT(
            project_built,
            std::weak_ptr<arcadia::project>
        );
        ARCADIA_EVENT(
            project_unbuilt
        );

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
