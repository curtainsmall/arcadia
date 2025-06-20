#pragma once

#include "core/event.hpp"
#include "core/math.hpp"

struct ACDA_API GLFWmonitor;

namespace Arcadia
{
    struct ACDA_API WindowLayer;

    enum struct ACDA_API WindowSizeState
    {
        Minimized = -1,
        Restored = 0,
        Maxmized = 1,
    };

    enum struct ACDA_API WindowCursorInputMode
    {
        Normal,
        Hidden,
        Disabled,
        Captured,
    };

    namespace Events
    {
        //==== Notifications ====//

        struct ACDA_API WindowShouldClose: public EventBase
        {
        };

        struct ACDA_API WindowCloseCanceled: public EventBase
        {
        };

        struct ACDA_API WindowSetSize: public EventBase
        {
        public:
            WindowSetSize(const glm::i32vec2& new_size):
                NewSize(new_size)
            {
            }
        public:
            const glm::i32vec2 NewSize;
        };

        struct ACDA_API WindowSetPosition: public EventBase
        {
        public:
            WindowSetPosition(const glm::i32vec2& new_size):
                NewPosition(new_size)
            {
            }
        public:
            const glm::i32vec2 NewPosition;
        };

        struct ACDA_API WindowSizeStateChanged: public EventBase
        {
        public:
            WindowSizeStateChanged(WindowSizeState state):
                State(state)
            {
            }
        public:
            const WindowSizeState State;
        };

        struct ACDA_API WindowSetFocused: public EventBase
        {
        public:
            WindowSetFocused(bool focused):
                Focused(focused)
            {
            }
        public:
            const bool Focused;
        };

        struct ACDA_API MonitorSetConnected: public EventBase
        {
        public:
            MonitorSetConnected(::GLFWmonitor* const glfw_monitor, bool connected):
                GlfwMonitor(glfw_monitor), Connected(connected)
            {
            }
        public:
            ::GLFWmonitor* const GlfwMonitor;
            const bool Connected;
        };

        //==== Adjustments ====//

        struct ACDA_API WindowSetCursorInputMode: public EventBase
        {
        public:
            WindowSetCursorInputMode(WindowCursorInputMode mode):
                Mode(mode)
            {
            }
        public:
            const WindowCursorInputMode Mode;
        };
    }
}