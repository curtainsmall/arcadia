#pragma once

#include"core/event/event.hpp"
#include"core/math.hpp"

class GLFWmonitor;

namespace Arcadia
{
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

        ACDA_DEFINE_EVENT(
            WindowShouldClose,
            WindowLayer* // Window to close
        );
        ACDA_DEFINE_EVENT(
            WindowCloseCanceled,
            WindowLayer* // Window to cancel close
        );

        ACDA_DEFINE_EVENT(
            WindowSetSize,
            WindowLayer*,
            glm::i32vec2 // New size
        );
        ACDA_DEFINE_EVENT(
            WindowSetPosition,
            WindowLayer*,
            glm::i32vec2 // New position
        );
        ACDA_DEFINE_EVENT(
            WindowSizeStateChanged,
            WindowLayer*,
            WindowSizeState // New state
        );
        ACDA_DEFINE_EVENT(
            WindowFocused,
            WindowLayer*,
            bool
        );

        ACDA_DEFINE_EVENT(
            MonitorConnect,
            ::GLFWmonitor*, // TODO: Use custom monitor type
            bool // Whether the monitor is connected or not
        );

        //==== Adjustments ====//

        ACDA_DEFINE_EVENT(
            WindowSetCursorInputMode,
            WindowCursorInputMode
        );
    }
}