#pragma once

#include <memory>

#include "platform/api_def.hpp"
#include "function/input/input_events.hpp"
#include "function/window/window_events.hpp"
#include "function/window/window_layer.hpp"
#include "ui/imgui.hpp"

namespace Arcadia
{
    namespace ImguiBackend
    {
        ACDA_API void Initialize(const std::shared_ptr<WindowLayer>& window_sptr);
        ACDA_API void NewFrame(const std::shared_ptr<WindowLayer>& window_sptr);
        ACDA_API void RenderDrawData(const std::shared_ptr<WindowLayer>& window_sptr);
        ACDA_API void Shutdown(const std::shared_ptr<WindowLayer>& window_sptr);

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