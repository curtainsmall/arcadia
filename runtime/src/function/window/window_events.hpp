#pragma once

#include"core/event/event.hpp"
#include"core/math.hpp"

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
                Window(window)
            {}
        public:
            WindowLayer* const Window;
        };

        class WindowCloseCanceled: public EventBase
        {
        public:
            WindowCloseCanceled(WindowLayer* const window):
                Window(window)
            {}
        public:
            WindowLayer* const Window;
        };

        class WindowSetSize: public EventBase
        {
        public:
            WindowSetSize(WindowLayer* const window, const glm::i32vec2& new_size):
                Window(window), NewSize(new_size)
            {}
        public:
            WindowLayer* const Window;
            const glm::i32vec2 NewSize;
        };

        class WindowSetPosition: public EventBase
        {
        public:
            WindowSetPosition(WindowLayer* const window, const glm::i32vec2& new_size):
                Window(window), NewPosition(new_size)
            {}
        public:
            WindowLayer* const Window;
            const glm::i32vec2 NewPosition;
        };

        class WindowSizeStateChanged: public EventBase
        {
        public:
            WindowSizeStateChanged(WindowLayer* const window, WindowSizeState state):
                Window(window), State(state)
            {}
        public:
            WindowLayer* const Window;
            const WindowSizeState State;
        };

        class WindowSetFocused: public EventBase
        {
        public:
            WindowSetFocused(WindowLayer* const window, bool focused):
                Window(window), Focused(focused)
            {}
        public:
            WindowLayer* const Window;
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