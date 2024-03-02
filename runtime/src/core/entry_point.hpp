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
    layer_stack.push_layer_at<arcadia::app_layer>(layer_stack.size() - 1, arcadia::create_application_uptr());

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

            for(auto& layer_uptr : std::ranges::reverse_view{ arcadia::layer_stack::instance() })
            {
                layer_uptr->on_event(event);
                if(event.handled)
                {
                    break;
                }

            }
            event_queue.pop();
        }

        // Updates
        for(auto& layer_uptr : arcadia::layer_stack::instance())
        {
            layer_uptr->on_update(app_context.timer.since_last());
        }
    }

    // Clear layer_stack
    layer_stack.pop_all();
    return 0;
}
