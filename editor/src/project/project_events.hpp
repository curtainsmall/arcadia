#pragma once

#include"core/event/event.hpp"

namespace arcadia
{
    struct project;

    namespace event
    {
        ARCADIA_EVENT(
            project_built,
            project*
        );
        ARCADIA_EVENT(
            project_unbuilt
        );
    }
}
