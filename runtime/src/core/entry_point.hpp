#pragma once

#include<memory>
#include<string>

#include"core/app/app_context.hpp"
#include"core/app/app_layer.hpp"
#include"core/base.hpp"
#include"core/layer/layer.hpp"

#if defined(_WIN32) && !defined(ACDA_IN_DEBUG)
#define ACDA_MAIN_FN_DECLARATION int WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
#define ACDA_MAIN_FN_DECL int main()
#endif // _WIN32

extern auto CreateApplication() -> std::unique_ptr<iAppLayer>;

ACDA_MAIN_FN_DECL
{
    // Add app_layer
    auto & layer_stack = LayerStack::Instance();
    layer_stack.PushLayer<iAppLayer>(layer_stack.end(), std::shared_ptr<iAppLayer>(CreateApplication()));

    // Main loop
    auto& app_context = AppContext::Instance();
    while(app_context.Running)
    {
        app_context.DeltaTime = app_context.Timer.SinceLast();

        // Process event 
        auto& event_queue = EventQueue::Instance();
        event_queue.SwapQueue();
        while(event_queue.Size())
        {
            auto& event = event_queue.Read();

            for(auto& layer : LayerStack::Instance())
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
        for(auto& layer : std::ranges::reverse_view{ LayerStack::Instance() })
        {
            layer->OnUpdate();
        }
    }

    // Clear layer_stack
    layer_stack.PopAll();
    return 0;
}
