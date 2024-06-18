#pragma once


#include"core/base.hpp"
#include"function/input/input_events.hpp"
#include"ui/imgui_header.hpp"
#include"function/window/window_events.hpp"
#include"function/window/window_layer.hpp"

namespace ImguiBackend
{
    ARCADIA_API void Initialize(const WindowLayer& window);
    ARCADIA_API void NewFrame(const WindowLayer& window);
    ARCADIA_API void RenderDrawData(const WindowLayer& window);
    ARCADIA_API void Shutdown(const WindowLayer& window);

    // Event handlers
    ARCADIA_API void ImguiOnEvent(EventBase& event);
    ARCADIA_API void ImguiOnWindowFocus(Event::WindowFocus& WindowFocus);
    ARCADIA_API void ImguiOnCursorEnter(Event::InputCursorEnter& input_cursor_enter);
    ARCADIA_API void ImguiOnCursorPos(Event::InputCursorPos& input_cursor_pos);
    ARCADIA_API void ImguiOnMouseButton(Event::InputMouseButton& input_mouse_button);
    ARCADIA_API void ImguiOnScroll(Event::InputScroll& input_scroll);
    ARCADIA_API void ImguiOnKey(Event::InputKey& input_key);
    ARCADIA_API void ImguiOnChar(Event::InputChar& input_char);
    ARCADIA_API void ImguiOnMonitor(Event::MonitorConnection& monitor_connection);
}
