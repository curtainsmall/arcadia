#pragma once

#include "core/event.hpp"
#include "core/math.hpp"

struct GLFWmonitor;

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

        class WindowShouldClose: public EventBase
        {
        public:
            WindowShouldClose(WindowLayer* const window):
                pWindowLayer(window)
            {}
        public:
            WindowLayer* const pWindowLayer;
        };

        class WindowCloseCanceled: public EventBase
        {
        public:
            WindowCloseCanceled(WindowLayer* const window):
                pWindowLayer(window)
            {}
        public:
            WindowLayer* const pWindowLayer;
        };

        class WindowSetSize: public EventBase
        {
        public:
            WindowSetSize(WindowLayer* const window, const glm::i32vec2& new_size):
                pWindowLayer(window), NewSize(new_size)
            {}
        public:
            WindowLayer* const pWindowLayer;
            const glm::i32vec2 NewSize;
        };

        class WindowSetPosition: public EventBase
        {
        public:
            WindowSetPosition(WindowLayer* const window, const glm::i32vec2& new_size):
                pWindowLayer(window), NewPosition(new_size)
            {}
        public:
            WindowLayer* const pWindowLayer;
            const glm::i32vec2 NewPosition;
        };

        class WindowSizeStateChanged: public EventBase
        {
        public:
            WindowSizeStateChanged(WindowLayer* const window, WindowSizeState state):
                pWindowLayer(window), State(state)
            {}
        public:
            WindowLayer* const pWindowLayer;
            const WindowSizeState State;
        };

        class WindowSetFocused: public EventBase
        {
        public:
            WindowSetFocused(WindowLayer* const window, bool focused):
                pWindowLayer(window), Focused(focused)
            {}
        public:
            WindowLayer* const pWindowLayer;
            const bool Focused;
        };

        class MonitorSetConnected: public EventBase
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

        class WindowSetCursorInputMode: public EventBase
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