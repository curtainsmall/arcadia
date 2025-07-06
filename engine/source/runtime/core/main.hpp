#pragma once

#include <memory>
#include <ranges>

#include "core/runtime_config.hpp"
#include "core/runtime_layer.hpp"
#include "core/event.hpp"
#include "core/layer.hpp"

#if defined(_WIN32) && !defined(ACDA_DEBUG_MODE)
#define ACDA_MAIN_FN_DECL int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
#define ACDA_MAIN_FN_DECL int main()
#endif // _WIN32

namespace Arcadia
{
    void ApplicationStartup();
}

extern void Arcadia::ApplicationStartup();
ACDA_MAIN_FN_DECL
{
    // Add runtime_layer
    Arcadia::LayerStack & layer_stack = Arcadia::LayerStack::Instance();
    layer_stack.PushLayer<Arcadia::RuntimeLayer>();

    Arcadia::ApplicationStartup();

    // Main loop
    std::shared_ptr<Arcadia::RuntimeLayer> runtime_layer_sptr = Arcadia::LayerStack::Instance().GetLayerShared<Arcadia::RuntimeLayer>();
    while(runtime_layer_sptr->IsRunning())
    {
        // Process event
        Arcadia::EventQueue& event_queue = Arcadia::EventQueue::Instance();
        event_queue.SwapQueue();
        while(event_queue.HasEvent())
        {
            for(const std::shared_ptr<Arcadia::LayerInterface>& layer_sptr : Arcadia::LayerStack::Instance())
            {
                bool event_handled = event_queue.ProcessEvent(
                    [&](Arcadia::EventBase& event) -> void
                    {
                        layer_sptr->OnEvent(event);
                    }
                );
                if(event_handled)
                {
                    break;
                }
            }
            event_queue.EventProcessFinished();
        }

        // Updates
        for(const std::shared_ptr<Arcadia::LayerInterface>& layer_sptr : std::ranges::reverse_view{ Arcadia::LayerStack::Instance() })
        {
            layer_sptr->OnUpdate();
        }
    }

    // Clear layer_stack
    layer_stack.PopAllLayers();
    return 0;
}