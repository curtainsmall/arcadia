#pragma once


#include"core/base.hpp"
#include"function/input/input_events.hpp"
#include"ui/imgui_header.hpp"
#include"function/window/window_events.hpp"
#include"function/window/window_layer.hpp"

namespace ImguiBackend
{
    ACDA_API void initialize(const WindowLayer& window);
    ACDA_API void new_frame(const WindowLayer& window);
    ACDA_API void render_draw_data(const WindowLayer& window);
    ACDA_API void shutdown(const WindowLayer& window);

    // Event handlers
    ACDA_API void imgui_on_event(EventBase& e);
    ACDA_API void imgui_on_window_focus(event::WindowFocus& WindowFocus);
    ACDA_API void imgui_on_cursor_enter(event::InputCursorEnter& input_cursor_enter);
    ACDA_API void imgui_on_cursor_pos(event::InputCursorPos& input_cursor_pos);
    ACDA_API void imgui_on_mouse_button(event::InputMouseButton& input_mouse_button);
    ACDA_API void imgui_on_scroll(event::InputScroll& input_scroll);
    ACDA_API void imgui_on_key(event::InputKey& input_key);
    ACDA_API void imgui_on_char(event::InputChar& input_char);
    ACDA_API void imgui_on_monitor(event::MonitorConnection& monitor_connection);
}
