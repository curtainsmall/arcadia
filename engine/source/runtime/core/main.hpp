#pragma once

#include <concepts>
#include <type_traits>

#include "core/runtime_config.hpp"
#include "core/runtime_layer.hpp"
#include "core/event.hpp"
#include "core/layer.hpp"

namespace Arcadia
{
    // This function including constructions, main loop and destructions
    // This function is seperated in case you want to declare your own main function while executing everything as normal
    template<Concepts::Layer AppLayer>
    ACDA_API auto Main() -> int
    {
        // Add runtime_layer
        LayerStack& layer_stack = LayerStack::Instance();
        layer_stack.PushLayer<RuntimeLayer>();

        layer_stack.PushLayer<AppLayer>();

        // Main loop
        std::shared_ptr<RuntimeLayer> runtime_layer_sptr = LayerStack::Instance().GetLayerShared<RuntimeLayer>();
        // To make sure all layers will be updated at least once and process events signaled in constructors
        do
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
            for(const std::shared_ptr<LayerInterface>& layer_sptr : std::ranges::reverse_view(LayerStack::Instance()))
            {
                layer_sptr->OnUpdate();
            }
        } while(runtime_layer_sptr->IsRunning());

        // Clear layer_stack
        layer_stack.PopAllLayers();
        return 0;
    }
}