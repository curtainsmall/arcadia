#pragma once

#include<memory>
#include<string>

#include"core/app/app_context.hpp"
#include"core/app/app_layer.hpp"
#include"core/base.hpp"
#include"core/layer/layer.hpp"

#if defined(_WIN32) && !defined(ACDA_DEBUG_MODE)
#define ACDA_MAIN_FN_DECLARATION int WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
#define ACDA_MAIN_FN_DECL int main()
#endif // _WIN32

extern auto Arcadia::CreateApplication()->std::unique_ptr<Arcadia::iAppLayer>;

ACDA_MAIN_FN_DECL
{
    // Add app_layer
    auto & layer_stack = Arcadia::LayerStack::Instance();
    layer_stack.PushLayer<Arcadia::iAppLayer>(layer_stack.end(), std::shared_ptr<Arcadia::iAppLayer>(Arcadia::CreateApplication()));

    // Main loop
    auto& app_context = Arcadia::AppContext::Instance();
    while(app_context.Running)
    {
        app_context.DeltaTime = app_context.Timer.Segment();

        // Process event
        auto& event_queue = Arcadia::EventQueue::Instance();
        event_queue.SwapQueue();
        while(event_queue.GetSize())
        {
            auto& event = event_queue.GetFront();

            for(auto& layer : Arcadia::LayerStack::Instance())
            {
                layer->OnEvent(event);
                if(event.Handled)
                {
                    break;
                }
            }
            event_queue.PopFront();
        }

        // Updates
        for(auto& layer : std::ranges::reverse_view{ Arcadia::LayerStack::Instance() })
        {
            layer->OnUpdate();
        }
    }

    // Clear layer_stack
    layer_stack.PopAllLayers();
    return 0;
}