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
        public:
            WindowShouldClose(WindowLayer* const window):
                pWindowLayer(window)
            {}
        public:
            WindowLayer* const pWindowLayer;
        };

        struct ACDA_API WindowCloseCanceled: public EventBase
        {
        public:
            WindowCloseCanceled(WindowLayer* const window):
                pWindowLayer(window)
            {}
        public:
            WindowLayer* const pWindowLayer;
        };

        struct ACDA_API WindowSetSize: public EventBase
        {
        public:
            WindowSetSize(WindowLayer* const window, const glm::i32vec2& new_size):
                pWindowLayer(window), NewSize(new_size)
            {}
        public:
            WindowLayer* const pWindowLayer;
            const glm::i32vec2 NewSize;
        };

        struct ACDA_API WindowSetPosition: public EventBase
        {
        public:
            WindowSetPosition(WindowLayer* const window, const glm::i32vec2& new_size):
                pWindowLayer(window), NewPosition(new_size)
            {}
        public:
            WindowLayer* const pWindowLayer;
            const glm::i32vec2 NewPosition;
        };

        struct ACDA_API WindowSizeStateChanged: public EventBase
        {
        public:
            WindowSizeStateChanged(WindowLayer* const window, WindowSizeState state):
                pWindowLayer(window), State(state)
            {}
        public:
            WindowLayer* const pWindowLayer;
            const WindowSizeState State;
        };

        struct ACDA_API WindowSetFocused: public EventBase
        {
        public:
            WindowSetFocused(WindowLayer* const window, bool focused):
                pWindowLayer(window), Focused(focused)
            {}
        public:
            WindowLayer* const pWindowLayer;
            const bool Focused;
        };

        struct ACDA_API MonitorSetConnected: public EventBase
        {
        public:
            MonitorSetConnected(::GLFWmonitor* const glfw_monitor, bool connected):
                GlfwMonitor(glfw_monitor), Connected(connected)
            {}
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
            {}
        public:
            const WindowCursorInputMode Mode;
        };
    }
}