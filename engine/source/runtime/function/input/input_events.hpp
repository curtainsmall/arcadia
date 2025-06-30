#pragma once

#include "GLFW/glfw3.h"

#include "core/event.hpp"
#include "core/math.hpp"
#include "function/window/window_layer.hpp"
#include "platform/defines.hpp"

namespace Arcadia
{
    enum struct ACDA_API InputKey
    {
        Unknown = GLFW_KEY_UNKNOWN,
        Space = GLFW_KEY_SPACE,
        Apostrophe = GLFW_KEY_APOSTROPHE, // '
        Comma = GLFW_KEY_COMMA, // ,
        Minus = GLFW_KEY_MINUS, // -
        Period = GLFW_KEY_PERIOD, // .
        Slash = GLFW_KEY_SLASH, // /
        Num0 = GLFW_KEY_0,
        Num1 = GLFW_KEY_1,
        Num2 = GLFW_KEY_2,
        Num3 = GLFW_KEY_3,
        Num4 = GLFW_KEY_4,
        Num5 = GLFW_KEY_5,
        Num6 = GLFW_KEY_6,
        Num7 = GLFW_KEY_7,
        Num8 = GLFW_KEY_8,
        Num9 = GLFW_KEY_9,
        Semicolon = GLFW_KEY_SEMICOLON, // ;
        Equal = GLFW_KEY_EQUAL, // =
        A = GLFW_KEY_A,
        B = GLFW_KEY_B,
        C = GLFW_KEY_C,
        D = GLFW_KEY_D,
        E = GLFW_KEY_E,
        F = GLFW_KEY_F,
        G = GLFW_KEY_G,
        H = GLFW_KEY_H,
        I = GLFW_KEY_I,
        J = GLFW_KEY_J,
        K = GLFW_KEY_K,
        L = GLFW_KEY_L,
        M = GLFW_KEY_M,
        N = GLFW_KEY_N,
        O = GLFW_KEY_O,
        P = GLFW_KEY_P,
        Q = GLFW_KEY_Q,
        R = GLFW_KEY_R,
        S = GLFW_KEY_S,
        T = GLFW_KEY_T,
        U = GLFW_KEY_U,
        V = GLFW_KEY_V,
        W = GLFW_KEY_W,
        X = GLFW_KEY_X,
        Y = GLFW_KEY_Y,
        Z = GLFW_KEY_Z,
        LeftBracket = GLFW_KEY_LEFT_BRACKET, // [
        Backslash = GLFW_KEY_BACKSLASH, /* \ */
        RightBracket = GLFW_KEY_RIGHT_BRACKET,
        GraveAccent = GLFW_KEY_GRAVE_ACCENT, // `
        World1 = GLFW_KEY_WORLD_1,
        World2 = GLFW_KEY_WORLD_2,
        Escape = GLFW_KEY_ESCAPE,
        Enter = GLFW_KEY_ENTER,
        Tab = GLFW_KEY_TAB,
        Backspace = GLFW_KEY_BACKSPACE,
        Insert = GLFW_KEY_INSERT,
        Delete = GLFW_KEY_DELETE,
        Right = GLFW_KEY_RIGHT,
        Left = GLFW_KEY_LEFT,
        Down = GLFW_KEY_DOWN,
        _UpAxis = GLFW_KEY_UP,
        PageUp = GLFW_KEY_PAGE_UP,
        PageDown = GLFW_KEY_PAGE_DOWN,
        Home = GLFW_KEY_HOME,
        End = GLFW_KEY_END,
        CapsLock = GLFW_KEY_CAPS_LOCK,
        ScrollLock = GLFW_KEY_SCROLL_LOCK,
        NumLock = GLFW_KEY_NUM_LOCK,
        PrintScreen = GLFW_KEY_PRINT_SCREEN,
        Pause = GLFW_KEY_PAUSE,
        F1 = GLFW_KEY_F1,
        F2 = GLFW_KEY_F2,
        F3 = GLFW_KEY_F3,
        F4 = GLFW_KEY_F4,
        F5 = GLFW_KEY_F5,
        F6 = GLFW_KEY_F6,
        F7 = GLFW_KEY_F7,
        F8 = GLFW_KEY_F8,
        F9 = GLFW_KEY_F9,
        F10 = GLFW_KEY_F10,
        F11 = GLFW_KEY_F11,
        F12 = GLFW_KEY_F12,
        Keypad0 = GLFW_KEY_KP_0,
        Keypad1 = GLFW_KEY_KP_1,
        Keypad2 = GLFW_KEY_KP_2,
        Keypad3 = GLFW_KEY_KP_3,
        Keypad4 = GLFW_KEY_KP_4,
        Keypad5 = GLFW_KEY_KP_5,
        Keypad6 = GLFW_KEY_KP_6,
        Keypad7 = GLFW_KEY_KP_7,
        Keypad8 = GLFW_KEY_KP_8,
        Keypad9 = GLFW_KEY_KP_9,
        Keypaddecimal = GLFW_KEY_KP_DECIMAL,
        Keypaddivide = GLFW_KEY_KP_DIVIDE,
        Keypadmultiply = GLFW_KEY_KP_MULTIPLY,
        Keypadsubtract = GLFW_KEY_KP_SUBTRACT,
        Keypadadd = GLFW_KEY_KP_ADD,
        Keypadenter = GLFW_KEY_KP_ENTER,
        Keypadequal = GLFW_KEY_KP_EQUAL,
        LeftShift = GLFW_KEY_LEFT_SHIFT,
        LeftCtrl = GLFW_KEY_LEFT_CONTROL,
        LeftAlt = GLFW_KEY_LEFT_ALT,
        LeftSuper = GLFW_KEY_LEFT_SUPER,
        RightShift = GLFW_KEY_RIGHT_SHIFT,
        RightCtrl = GLFW_KEY_RIGHT_CONTROL,
        RightAlt = GLFW_KEY_RIGHT_ALT,
        RightSuper = GLFW_KEY_RIGHT_SUPER,
        Menu = GLFW_KEY_MENU,
    };
    enum struct ACDA_API InputMouse
    {
        ButtonLeft = GLFW_MOUSE_BUTTON_LEFT,
        ButtonRight = GLFW_MOUSE_BUTTON_RIGHT,
        ButtonMiddle = GLFW_MOUSE_BUTTON_MIDDLE,
    };
    enum struct ACDA_API InputAction
    {
        Release = GLFW_RELEASE,
        Press = GLFW_PRESS,
        Repeat = GLFW_REPEAT,
    };
    enum struct ACDA_API InputModifier
    {
        Shift = GLFW_MOD_SHIFT,
        Ctrl = GLFW_MOD_CONTROL,
        Alt = GLFW_MOD_ALT,
        Super = GLFW_MOD_SUPER,
        CapsLock = GLFW_MOD_CAPS_LOCK,
        NumLock = GLFW_MOD_NUM_LOCK,
        _EnumBitfield
    };

    namespace Events
    {
        struct ACDA_API InputKey: public EventBase
        {
        public:
            InputKey(
                std::int32_t key_code,
                std::int32_t key_scancode,
                std::int32_t action,
                std::int32_t modifier
            ):
                KeyCode(key_code),
                KeyScancode(key_scancode),
                Action(action),
                Modifier(modifier)
            {
            }
        public:
            const std::int32_t KeyCode;
            const std::int32_t KeyScancode;
            const std::int32_t Action;
            const std::int32_t Modifier;
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
            InputScroll(float scroll_offet_vertical, float scroll_offset_horizontal):
                ScrollOffsetVertical(scroll_offet_vertical), ScrollOffsetHorizontal(scroll_offset_horizontal)
            {
            }
        public:
            const float ScrollOffsetVertical;
            const float ScrollOffsetHorizontal;
        };

        struct ACDA_API InputMouseButton: public EventBase
        {
        public:
            InputMouseButton(std::int32_t code, std::int32_t action, std::int32_t modifier):
                Code(code), Action(action), Modifier(modifier)
            {
            }
        public:
            const std::int32_t Code;
            const std::int32_t Action;
            const std::int32_t Modifier;
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