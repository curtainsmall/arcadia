#pragma once

#include"core/event/event.hpp"
#include"resource/scene/scene.hpp"

namespace arcadia
{
    struct project;

    namespace event
    {
        ARCADIA_EVENT(
            project_built,
            arcadia::project*
        );
        ARCADIA_EVENT(
            project_unbuilt
        );

        ARCADIA_EVENT(
            scene_built,
            arcadia::scene*
        );
        ARCADIA_EVENT(
            scene_unbuilt
        );
    }
}
