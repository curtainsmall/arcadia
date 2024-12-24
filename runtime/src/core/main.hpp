#pragma once

#include<memory>
#include<string>

#include"core/app/app_context.hpp"
#include"core/app/app_layer.hpp"
#include"core/layer/layer.hpp"
#include"platform/api_def.hpp"
#include"platform/debug_def.hpp"

#if defined(_WIN32) && !defined(ACDA_DEBUG_MODE)
#define ACDA_MAIN_FN_DECL int WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
#define ACDA_MAIN_FN_DECL int main()
#endif // _WIN32

extern auto Arcadia::CreateApplication()->std::unique_ptr<Arcadia::AppLayerInterface>;

ACDA_MAIN_FN_DECL
{
    // Add app_layer
    Arcadia::LayerStack & layer_stack = Arcadia::LayerStack::Instance();
    layer_stack.PushLayer<Arcadia::AppLayerInterface>(layer_stack.end(), std::shared_ptr<Arcadia::AppLayerInterface>(Arcadia::CreateApplication()));

    // Main loop
    Arcadia::AppContext& app_context = Arcadia::AppContext::Instance();
    while(app_context.Running)
    {
        app_context.DeltaTime = app_context.Timer.Segment();

        // Process event
        Arcadia::EventQueue& event_queue = Arcadia::EventQueue::Instance();
        event_queue.SwapQueue();
        while(event_queue.GetSize())
        {
            Arcadia::EventBase& event = event_queue.GetFront();

            for(const std::shared_ptr<Arcadia::LayerInterface>& layer_sptr : Arcadia::LayerStack::Instance())
            {
                layer_sptr->OnEvent(event);
                if(event.IsHandled())
                {
                    break;
                }
            }
            event_queue.PopFront();
        }

        // Updates
        for(const std::shared_ptr<Arcadia::LayerInterface>& layer : std::ranges::reverse_view{ Arcadia::LayerStack::Instance() })
        {
            layer->OnUpdate();
        }
    }

    // Clear layer_stack
    layer_stack.PopAllLayers();
    return 0;
}