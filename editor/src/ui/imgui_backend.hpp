#pragma once


#include"core/base.hpp"
#include"function/input/input_events.hpp"
#include"ui/imgui_header.hpp"
#include"function/window/window_events.hpp"
#include"function/window/window_layer.hpp"

namespace ImguiBackend
{
    ACDA_API void Initialize(const WindowLayer& window);
    ACDA_API void NewFrame(const WindowLayer& window);
    ACDA_API void RenderDrawData(const WindowLayer& window);
    ACDA_API void Shutdown(const WindowLayer& window);

    // Event handlers
    ACDA_API void ImguiOnEvent(EventBase& event);
    ACDA_API void ImguiOnWindowFocus(Event::WindowFocus& WindowFocus);
    ACDA_API void ImguiOnCursorEnter(Event::InputCursorEnter& input_cursor_enter);
    ACDA_API void ImguiOnCursorPos(Event::InputCursorPos& input_cursor_pos);
    ACDA_API void ImguiOnMouseButton(Event::InputMouseButton& input_mouse_button);
    ACDA_API void ImguiOnScroll(Event::InputScroll& input_scroll);
    ACDA_API void ImguiOnKey(Event::InputKey& input_key);
    ACDA_API void ImguiOnChar(Event::InputChar& input_char);
    ACDA_API void ImguiOnMonitor(Event::MonitorConnection& monitor_connection);
}
