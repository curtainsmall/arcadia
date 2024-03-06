#pragma once


#include"core/base.hpp"
#include"function/input/input_events.hpp"
#include"function/ui/imgui_header.hpp"
#include"function/window/window_events.hpp"
#include"function/window/window_layer.hpp"

namespace arcadia
{
    namespace imgui_backend
    {
        ARCADIA_API void initialize(const arcadia::window_layer& window);
        ARCADIA_API void begin_frame(const arcadia::window_layer& window);
        ARCADIA_API void render_draw_data(const arcadia::window_layer& window);
        ARCADIA_API void shutdown(const arcadia::window_layer& window);

        // Event handlers
        ARCADIA_API void imgui_on_event(arcadia::event_base& event);
        ARCADIA_API void imgui_on_window_focus(arcadia::event::window_focus& window_focus);
        ARCADIA_API void imgui_on_cursor_enter(arcadia::event::input_cursor_enter& input_cursor_enter);
        ARCADIA_API void imgui_on_cursor_pos(arcadia::event::input_cursor_pos& input_cursor_pos);
        ARCADIA_API void imgui_on_mouse_button(arcadia::event::input_mouse_button& input_mouse_button);
        ARCADIA_API void imgui_on_scroll(arcadia::event::input_scroll& input_scroll);
        ARCADIA_API void imgui_on_key(arcadia::event::input_key& input_key);
        ARCADIA_API void imgui_on_char(arcadia::event::input_char& input_char);
        ARCADIA_API void imgui_on_monitor(arcadia::event::monitor_connection& monitor_connection);
    }
}
