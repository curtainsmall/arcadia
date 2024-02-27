#pragma once

#include"core/event/event.hpp"
#include"core/math.hpp"

namespace arcadia
{
    struct window_layer;

    namespace event
    {
        ARCADIA_EVENT(
            window_close,
            arcadia::window_layer* // Window to close
        );
        ARCADIA_EVENT(
            window_size,
            arcadia::window_layer*,
            glm::ivec2 // New size
        );
        ARCADIA_EVENT(
            window_pos,
            arcadia::window_layer*,
            glm::ivec2 // New position
        );
        ARCADIA_EVENT(
            window_minified,
            arcadia::window_layer*
        );
        ARCADIA_EVENT(
            window_restored,
            arcadia::window_layer*
        );
        ARCADIA_EVENT(
            window_maxmized,
            arcadia::window_layer*
        );
        ARCADIA_EVENT(
            window_focus,
            arcadia::window_layer*,
            bool
        );
    }
}
