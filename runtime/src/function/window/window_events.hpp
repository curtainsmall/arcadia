#pragma once

#include"core/event/event.hpp"
#include"core/math.hpp"

struct GLFWmonitor;

namespace Arcadia
{
    struct WindowLayer;

    enum struct WindowSizeState: std::int8_t
    {
        Minimized = -1,
        Restored = 0,
        Maxmized = 1,
    };

    namespace Event
    {
        ARCADIA_EVENT(
            WindowShouldClose,
            Arcadia::WindowLayer* // Window to close
        );
        ARCADIA_EVENT(
            WindowCloseCanceled,
            Arcadia::WindowLayer* // Window to cancel close
        );

        ARCADIA_EVENT(
            WindowSize,
            Arcadia::WindowLayer*,
            glm::ivec2 // New size
        );
        ARCADIA_EVENT(
            WindowPos,
            Arcadia::WindowLayer*,
            glm::ivec2 // New position
        );
        ARCADIA_EVENT(
            WindowSizeState,
            Arcadia::WindowLayer*,
            Arcadia::WindowSizeState // New state
        );
        ARCADIA_EVENT(
            WindowFocus,
            Arcadia::WindowLayer*,
            bool
        );

        ARCADIA_EVENT(
            MonitorConnection,
            GLFWmonitor*, // TODO: Use custom monitor type
            bool // Whether the monitor is connected or not
        );
    }
}
