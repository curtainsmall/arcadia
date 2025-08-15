#pragma once

#include "core/event.hpp"
#include "core/math.hpp"
#include "platform/glfw.hpp"

struct ACDA_API GLFWmonitor;

namespace Arcadia
{
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
            WindowSizeStateChanged(GlfwWindowSizeState state):
                State(state)
            {
            }
        public:
            const GlfwWindowSizeState State;
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
            WindowSetCursorInputMode(GlfwWindowCursorInputMode mode):
                Mode(mode)
            {
            }
        public:
            const GlfwWindowCursorInputMode Mode;
        };

        struct ACDA_API InputKey: public EventBase
        {
        public:
            InputKey(
                GlfwInputKey key_code,
                std::int32_t key_scancode,
                GlfwInputAction action,
                GlfwInputModifier modifier
            ):
                KeyCode(key_code),
                KeyScancode(key_scancode),
                Action(action),
                Modifier(modifier)
            {
            }
        public:
            const GlfwInputKey KeyCode;
            const std::int32_t KeyScancode;
            const GlfwInputAction Action;
            const GlfwInputModifier Modifier;
        };

        struct ACDA_API InputCursorPosition: public EventBase
        {
        public:
            InputCursorPosition(const glm::vec2& cursor_position):
                CursorPosition(cursor_position)
            {
            }
        public:
            const glm::vec2 CursorPosition;
        };

        struct ACDA_API InputCursorMove: public EventBase
        {
        public:
            InputCursorMove(const glm::vec2 cursor_move_distance):
                CursorMoveDistance(cursor_move_distance)
            {
            }
        public:
            const glm::vec2 CursorMoveDistance;
        };

        struct ACDA_API InputScroll: public EventBase
        {
        public:
            InputScroll(float scroll_offet_x, float scroll_offset_y):
                ScrollOffsetX(scroll_offet_x), ScrollOffsetY(scroll_offset_y)
            {
            }
        public:
            const float ScrollOffsetX;
            const float ScrollOffsetY;
        };

        struct ACDA_API InputMouseButton: public EventBase
        {
        public:
            InputMouseButton(GlfwInputMouseButton mouse_button, GlfwInputAction action, GlfwInputModifier modifier):
                MouseButton(mouse_button), Action(action), Modifier(modifier)
            {
            }
        public:
            const GlfwInputMouseButton MouseButton;
            const GlfwInputAction Action;
            const GlfwInputModifier Modifier;
        };

        struct ACDA_API InputCursorEnter: public EventBase
        {
        public:
            InputCursorEnter(bool entered):
                Entered(entered)
            {
            }
        public:
            const bool Entered;
        };

        struct ACDA_API InputChar: public EventBase
        {
        public:
            InputChar(std::uint32_t unicode_codepoint):
                UnicodeCodepoint(unicode_codepoint)
            {
            }
        public:
            const std::uint32_t UnicodeCodepoint;
        };
    }
}