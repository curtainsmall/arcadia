#pragma once


#include"core/base.hpp"
#include"function/input/input_events.hpp"
#include"ui/imgui_header.hpp"
#include"function/window/window_events.hpp"
#include"function/window/window_layer.hpp"

namespace imgui_backend
{
    ACDA_API void initialize(const WindowLayer& window);
    ACDA_API void new_frame(const WindowLayer& window);
    ACDA_API void render_draw_data(const WindowLayer& window);
    ACDA_API void shutdown(const WindowLayer& window);

    // Event handlers
    ACDA_API void on_event(EventBase& e);
    ACDA_API void on_window_focus(events::WindowFocus& WindowFocus);
    ACDA_API void on_cursor_enter(events::InputCursorEnter& input_cursor_enter);
    ACDA_API void on_cursor_pos(events::InputCursorPos& input_cursor_pos);
    ACDA_API void on_mouse_button(events::InputMouseButton& input_mouse_button);
    ACDA_API void on_scroll(events::InputScroll& input_scroll);
    ACDA_API void on_key(events::InputKey& input_key);
    ACDA_API void on_char(events::InputChar& input_char);
    ACDA_API void on_monitor(events::MonitorConnection& monitor_connection);
}
