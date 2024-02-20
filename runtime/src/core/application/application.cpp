#include"pch.hpp"
#include"application.hpp"

#include<ranges>

void arcadia::application::run()
{
    while(_running)
    {
        // Process event 
        auto& event_queue = arcadia::event_queue::instance();
        event_queue.swap_queue();
        while(event_queue.non_empty())
        {
            auto& event = event_queue.read();

            for(auto& layer_uptr : std::ranges::reverse_view{ arcadia::layer_stack::instance() })
            {
                if(!layer_uptr->on_event(event))
                {
                    break;
                }
            }
            event_queue.pop();
        }

        // Updates
        for(auto& layer_uptr : arcadia::layer_stack::instance())
        {
            layer_uptr->on_update(_timer.since_last());
        }
    }
}
