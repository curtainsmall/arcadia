#pragma once

#include"core/event/event.hpp"
#include"core/math.hpp"

struct GLFWmonitor;

namespace arcadia
{
    struct window_layer;

    enum struct window_size_state: int8_t
    {
        minimized = -1,
        restored = 0,
        maxmized = 1,
    };

    namespace event
    {
        ARCADIA_EVENT(
            window_should_close,
            arcadia::window_layer* // Window to close
        );
        ARCADIA_EVENT(
            window_close_canceled,
            arcadia::window_layer* // Window to cancel close
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
            window_size_state,
            arcadia::window_layer*,
            arcadia::window_size_state // New state
        );
        ARCADIA_EVENT(
            window_focus,
            arcadia::window_layer*,
            bool
        );

        ARCADIA_EVENT(
            monitor_connection,
            GLFWmonitor*, // TODO: Use custom monitor type
            bool // Whether the monitor is connected or not
        );
    }
}
