#pragma once

#include <memory>


#include "function/window/window_events.hpp"
#include "function/window/window_layer.hpp"
#include "ui/imgui.hpp"

namespace Arcadia
{
    namespace ImguiBackend
    {
        void Initialize(const std::shared_ptr<WindowLayer>& window_sptr);
        void NewFrame(const std::shared_ptr<WindowLayer>& window_sptr);
        void RenderDrawData(const std::shared_ptr<WindowLayer>& window_sptr);
        void Shutdown(const std::shared_ptr<WindowLayer>& window_sptr);

        // Event handlers
        void OnEvent(EventBase& e);
        void OnWindowSetFocused(Events::WindowSetFocused& e);
        void OnInputCursorEnter(Events::InputCursorEnter& e);
        void OnInputCursorPosition(Events::InputCursorPosition& e);
        void OnInputMouseButton(Events::InputMouseButton& e);
        void OnInputScroll(Events::InputScroll& e);
        void OnInputKey(Events::InputKey& e);
        void OnInputChar(Events::InputChar& e);
        void OnMonitorSetConnected(Events::MonitorSetConnected& e);
    }
}