#pragma once

#include"core/base.hpp"
#include"function/input/input_events.hpp"
#include"function/window/window_events.hpp"
#include"function/window/window_layer.hpp"
#include"ui/imgui_header.hpp"

namespace Arcadia
{
    namespace ImguiBackend
    {
        ACDA_API void Initialize(const WindowLayer& window);
        ACDA_API void NewFrame(const WindowLayer& window);
        ACDA_API void RenderDrawData(const WindowLayer& window);
        ACDA_API void Shutdown(const WindowLayer& window);

        // Event handlers
        ACDA_API void OnEvent(EventBase& e);
        ACDA_API void OnWindowFocus(Events::WindowFocused& window_focused);
        ACDA_API void OnInputCursorEnter(Events::InputCursorEnter& input_cursor_enter);
        ACDA_API void OnInputCursorPos(Events::InputCursorPos& input_cursor_pos);
        ACDA_API void OnInputMouseButton(Events::InputMouseButton& input_mouse_button);
        ACDA_API void OnInputScroll(Events::InputScroll& input_scroll);
        ACDA_API void OnInputKey(Events::InputKey& input_key);
        ACDA_API void OnInputChar(Events::InputChar& input_char);
        ACDA_API void OnMonitorConnect(Events::MonitorConnect& monitor_connect);
    }
}