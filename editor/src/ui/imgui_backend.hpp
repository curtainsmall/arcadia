#pragma once

#include "platform/api_def.hpp"
#include "function/input/input_events.hpp"
#include "function/window/window_events.hpp"
#include "function/window/window_layer.hpp"
#include "ui/imgui.hpp"

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
        ACDA_API void OnWindowSetFocused(Events::WindowSetFocused& e);
        ACDA_API void OnInputCursorEnter(Events::InputCursorEnter& e);
        ACDA_API void OnInputCursorPosition(Events::InputCursorPosition& e);
        ACDA_API void OnInputMouseButton(Events::InputMouseButton& e);
        ACDA_API void OnInputScroll(Events::InputScroll& e);
        ACDA_API void OnInputKey(Events::InputKey& e);
        ACDA_API void OnInputChar(Events::InputChar& e);
        ACDA_API void OnMonitorSetConnected(Events::MonitorSetConnected& e);
    }
}