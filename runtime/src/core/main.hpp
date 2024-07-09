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

extern auto create_application() -> std::unique_ptr<iAppLayer>;

ACDA_MAIN_FN_DECL
{
    // Add app_layer
    auto & layer_stack = LayerStack::instance();
    layer_stack.push_layer<iAppLayer>(layer_stack.end(), std::shared_ptr<iAppLayer>(create_application()));

    // Main loop
    auto& app_context = AppContext::instance();
    while(app_context.running)
    {
        app_context.delta_time = app_context.timer.since_last();

        // Process event 
        auto& event_queue = EventQueue::instance();
        event_queue.swap_queue();
        while(event_queue.size())
        {
            auto& event = event_queue.read();

            for(auto& layer : LayerStack::instance())
            {
                layer->on_event(event);
                if(event.handled)
                {
                    break;
                }

            }
            event_queue.pop();
        }

        // Updates
        for(auto& layer : std::ranges::reverse_view{ LayerStack::instance() })
        {
            layer->on_update();
        }
    }

    // Clear layer_stack
    layer_stack.pop_all();
    return 0;
}
