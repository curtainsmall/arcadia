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

    ACDA_EVENT(
        WindowShouldClose,
        WindowLayer* // Window to close
    );
    ACDA_EVENT(
        WindowCloseCanceled,
        WindowLayer* // Window to cancel close
    );

    ACDA_EVENT(
        WindowSize,
        WindowLayer*,
        glm::ivec2 // New size
    );
    ACDA_EVENT(
        WindowPos,
        WindowLayer*,
        glm::ivec2 // New position
    );
    ACDA_EVENT(
        WindowSizeState,
        WindowLayer*,
        ::WindowSizeState // New state
    );
    ACDA_EVENT(
        WindowFocus,
        WindowLayer*,
        bool
    );

    ACDA_EVENT(
        MonitorConnection,
        GLFWmonitor*, // TODO: Use custom monitor type
        bool // Whether the monitor is connected or not
    );

    //==== Adjustments ====//

    ACDA_EVENT(
        WindowSetInputModeCursor,
        WindowInputModeCursor
    );
}
