#pragma once

#include <memory>
#include <ranges>

#include "core/app/app_context.hpp"
#include "core/app/app_layer.hpp"
#include "core/event.hpp"
#include "core/layer.hpp"
#include "platform/debug_def.hpp"

#if defined(_WIN32) && !defined(ACDA_DEBUG_MODE)
#define ACDA_MAIN_FN_DECL int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
#define ACDA_MAIN_FN_DECL int main()
#endif // _WIN32

extern auto Arcadia::CreateApplicationUnique()->std::unique_ptr<Arcadia::AppLayerInterface>;

ACDA_MAIN_FN_DECL
{
    // Add app_layer
    Arcadia::LayerStack & layer_stack = Arcadia::LayerStack::Instance();
    layer_stack.PushLayer<Arcadia::AppLayerInterface>(
        layer_stack.end(),
        std::shared_ptr<Arcadia::AppLayerInterface>(Arcadia::CreateApplicationUnique())
    );

    // Main loop
    Arcadia::AppContext& app_context = Arcadia::AppContext::Instance();
    while(app_context.Running)
    {
        app_context.DeltaTime = app_context.Timer.Segment();

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