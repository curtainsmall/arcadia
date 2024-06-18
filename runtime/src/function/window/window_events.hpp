#pragma once

#include"core/event/event.hpp"
#include"core/math.hpp"

struct GLFWmonitor;

struct WindowLayer;

enum struct WindowSizeState
{
    Minimized = -1,
    Restored = 0,
    Maxmized = 1,
};

enum struct WindowInputModeCursor
{
    Normal,
    Hidden,
    Disabled,
    Captured,
};

namespace Event
{
    //==== Notifications ====//

    ARCADIA_EVENT(
        WindowShouldClose,
        WindowLayer* // Window to close
    );
    ARCADIA_EVENT(
        WindowCloseCanceled,
        WindowLayer* // Window to cancel close
    );

    ARCADIA_EVENT(
        WindowSize,
        WindowLayer*,
        glm::ivec2 // New size
    );
    ARCADIA_EVENT(
        WindowPos,
        WindowLayer*,
        glm::ivec2 // New position
    );
    ARCADIA_EVENT(
        WindowSizeState,
        WindowLayer*,
        ::WindowSizeState // New state
    );
    ARCADIA_EVENT(
        WindowFocus,
        WindowLayer*,
        bool
    );

    ARCADIA_EVENT(
        MonitorConnection,
        GLFWmonitor*, // TODO: Use custom monitor type
        bool // Whether the monitor is connected or not
    );

    //==== Adjustments ====//

    ARCADIA_EVENT(
        WindowSetInputModeCursor,
        WindowInputModeCursor
    );
}
