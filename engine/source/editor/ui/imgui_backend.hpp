#pragma once

#include <memory>


#include "app/engine.hpp"
#include "function/window/window_events.hpp"
#include "function/window/window_service.hpp"
#include "ui/imgui.hpp"

namespace Arcadia
{
    namespace ImguiBackend
    {
        void Initialize(Engine& engine, WindowService& window);
        void NewFrame(Engine& engine, WindowService& window);
        void RenderDrawData(Engine& engine, WindowService& window);
        void Shutdown(Engine& engine, WindowService& window);

        // Event handlers
        void OnEvent(Engine& engine, EventBase& e);
    }
}
