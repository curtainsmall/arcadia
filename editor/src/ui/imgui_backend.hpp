#pragma once


#include"core/base.hpp"
#include"function/input/input_events.hpp"
#include"ui/imgui_header.hpp"
#include"function/window/window_events.hpp"
#include"function/window/window_layer.hpp"

namespace Arcadia
{
    namespace ImguiBackend
    {
        ARCADIA_API void Initialize(const Arcadia::WindowLayer& window);
        ARCADIA_API void NewFrame(const Arcadia::WindowLayer& window);
        ARCADIA_API void RenderDrawData(const Arcadia::WindowLayer& window);
        ARCADIA_API void Shutdown(const Arcadia::WindowLayer& window);

        // Event handlers
        ARCADIA_API void ImguiOnEvent(Arcadia::EventBase& event);
        ARCADIA_API void ImguiOnWindowFocus(Arcadia::Event::WindowFocus& WindowFocus);
        ARCADIA_API void ImguiOnCursorEnter(Arcadia::Event::InputCursorEnter& input_cursor_enter);
        ARCADIA_API void ImguiOnCursorPos(Arcadia::Event::InputCursorPos& input_cursor_pos);
        ARCADIA_API void ImguiOnMouseButton(Arcadia::Event::InputMouseButton& input_mouse_button);
        ARCADIA_API void ImguiOnScroll(Arcadia::Event::InputScroll& input_scroll);
        ARCADIA_API void ImguiOnKey(Arcadia::Event::InputKey& input_key);
        ARCADIA_API void ImguiOnChar(Arcadia::Event::InputChar& input_char);
        ARCADIA_API void ImguiOnMonitor(Arcadia::Event::MonitorConnection& monitor_connection);
    }
}
