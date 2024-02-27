#pragma once


#include"core/base.hpp"
#include"function/input/input_events.hpp"
#include"function/window/monitor.hpp"
#include"function/window/window_events.hpp"
#include"function/window/window_layer.hpp"
#include"ui/imgui_header.hpp"

namespace arcadia
{
    namespace imgui_backend
    {
        ARCADIA_API void initialize(const arcadia::window_layer& window);
        ARCADIA_API void new_frame(const arcadia::window_layer& window);
        ARCADIA_API void render_draw_data(const arcadia::window_layer& window);
        ARCADIA_API void shutdown(const arcadia::window_layer& window);

        // Event handlers
        ARCADIA_API auto imgui_on_event(const arcadia::event_base& event) -> bool;
        ARCADIA_API auto imgui_on_window_focus(const arcadia::event::window_focus& window_focus) -> bool;
        ARCADIA_API auto imgui_on_cursor_enter(const arcadia::event::input_cursor_enter& input_cursor_enter) -> bool;
        ARCADIA_API auto imgui_on_cursor_pos(const arcadia::event::input_cursor_pos& input_cursor_pos) -> bool;
        ARCADIA_API auto imgui_on_mouse_button(const arcadia::event::input_mouse_button& input_mouse_button) -> bool;
        ARCADIA_API auto imgui_on_scroll(const arcadia::event::input_scroll& input_scroll) -> bool;
        ARCADIA_API auto imgui_on_key(const arcadia::event::input_key& input_key) -> bool;
        ARCADIA_API auto imgui_on_char(const arcadia::event::input_char& input_char) -> bool;
        ARCADIA_API auto imgui_on_monitor(const arcadia::event::monitor_connection& monitor_connection) -> bool;
    }
}
