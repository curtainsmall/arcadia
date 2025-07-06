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
    using namespace Arcadia;

// Add runtime_layer
LayerStack& layer_stack = LayerStack::Instance();
layer_stack.PushLayer<RuntimeLayer>();

ApplicationStartup();

// Main loop
std::shared_ptr<RuntimeLayer> runtime_layer_sptr = LayerStack::Instance().GetLayerShared<RuntimeLayer>();
while(runtime_layer_sptr->IsRunning())
{
    // Process event
    EventQueue& event_queue = EventQueue::Instance();
    event_queue.SwapQueue();
    while(event_queue.HasEvent())
    {
        for(const std::shared_ptr<LayerInterface>& layer_sptr : LayerStack::Instance())
        {
            bool event_handled = event_queue.ProcessEvent(
                [&](EventBase& event) -> void
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
    for(const std::shared_ptr<LayerInterface>& layer_sptr : std::ranges::reverse_view{ LayerStack::Instance() })
    {
        layer_sptr->OnUpdate();
    }
}

// Clear layer_stack
layer_stack.PopAllLayers();
return 0;
}