#pragma once

#include<memory>
#include<string>

#include"core/app/app_context.hpp"
#include"core/app/app_layer.hpp"
#include"core/base.hpp"
#include"core/layer/layer.hpp"

extern auto create_application_uptr() -> std::unique_ptr<arcadia::app_layer>;

auto main(
    int argc,
    const char** argv
) -> int
{
    // Add app_layer
    auto& layer_stack = arcadia::layer_stack::instance();
    layer_stack.push_layer<arcadia::app_layer>(layer_stack.end(), std::shared_ptr<arcadia::app_layer>(arcadia::create_application_uptr()));

    // Main loop
    auto& app_context = arcadia::app_context::instance();
    while(app_context.running)
    {
        // Process event 
        auto& event_queue = arcadia::event_queue::instance();
        event_queue.swap_queue();
        while(event_queue.size())
        {
            auto& event = event_queue.read();

            for(auto& layer_sptr : arcadia::layer_stack::instance())
            {
                layer_sptr->on_event(event);
                if(event.handled)
                {
                    break;
                }

            }
            event_queue.pop();
        }

        // Updates
        for(auto& layer_sptr : std::ranges::reverse_view{ arcadia::layer_stack::instance() })
        {
            layer_sptr->on_update(app_context.timer.since_last());
        }
    }

    // Clear layer_stack
    layer_stack.pop_all();
    return 0;
}
