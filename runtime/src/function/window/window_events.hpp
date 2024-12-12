#pragma once

#include"core/event/event.hpp"
#include"core/math.hpp"

class GLFWmonitor;

class WindowLayer;

enum class WindowSizeState
{
    Minimized = -1,
    Restored = 0,
    Maxmized = 1,
};

enum class WindowCursorInputMode
{
    Normal,
    Hidden,
    Disabled,
    Captured,
};

namespace Events
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
        WindowPosition,
        WindowLayer*,
        glm::ivec2 // New position
    );
    ACDA_EVENT(
        WindowSizeState,
        WindowLayer*,
        ::WindowSizeState // New state
    );
    ACDA_EVENT(
        WindowFocused,
        WindowLayer*,
        bool
    );

    ACDA_EVENT(
        MonitorConnect,
        GLFWmonitor*, // TODO: Use custom monitor type
        bool // Whether the monitor is connected or not
    );

    //==== Adjustments ====//

    ACDA_EVENT(
        WindowSetCursorInputMode,
        WindowCursorInputMode
    );
}