#pragma once

#include<memory>
#include<string>

#include"core/app/app_context.hpp"
#include"core/app/app_layer.hpp"
#include"core/base.hpp"
#include"core/layer/layer.hpp"

extern auto CreateApplicationUptr() -> std::unique_ptr<Arcadia::iAppLayer>;

auto main() -> int
{
    // Add app_layer
    auto& layer_stack = Arcadia::LayerStack::Instance();
    layer_stack.PushLayer<Arcadia::iAppLayer>(layer_stack.end(), std::shared_ptr<Arcadia::iAppLayer>(Arcadia::CreateApplicationUptr()));

    // Main loop
    auto& app_context = Arcadia::AppContext::Instance();
    while(app_context.Running)
    {
        app_context.DeltaTime = app_context.Timer.SinceLast();

        // Process event 
        auto& event_queue = Arcadia::EventQueue::Instance();
        event_queue.SwapQueue();
        while(event_queue.Size())
        {
            auto& event = event_queue.Read();

            for(auto& layer : Arcadia::LayerStack::Instance())
            {
                layer->OnEvent(event);
                if(event.Handled)
                {
                    break;
                }

            }
            event_queue.Pop();
        }

        // Updates
        for(auto& layer : std::ranges::reverse_view{ Arcadia::LayerStack::Instance() })
        {
            layer->OnUpdate();
        }
    }

    // Clear layer_stack
    layer_stack.PopAll();
    return 0;
}
