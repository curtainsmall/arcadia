#include"pch.hpp"
#include"app.hpp"

#include<ranges>

#include"core/log/log.hpp"

#ifndef NDEBUG
#include"function/input/input.hpp"
#endif

arcadia::app_layer::app_layer():
    arcadia::layer_interface("application")
{}

void arcadia::app_layer::run()
{
    while(_running)
    {
        // Process event 
        auto& event_queue = arcadia::event_queue::instance();
        event_queue.swap_queue();
        while(event_queue.size())
        {
            auto& event = event_queue.read();

        #ifndef NDEBUG
            _debug_log_event(event);
        #endif
            for(auto& layer_uptr : std::ranges::reverse_view{ arcadia::layer_stack::instance() })
            {
                if(layer_uptr->on_event(event))
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

#ifndef NDEBUG
void arcadia::app_layer::_debug_log_event(const arcadia::event_base& event)
{
    auto log_when_cursor_pos = [](const arcadia::input_cursor_pos& e) -> bool
    {
        const auto& [wnd_ptr, pos] = e.data_tuple;
        arcadia::log::debug(std::format("Pos: {}, {}", pos.x, pos.y));
        return false;
    };
    auto log_when_cursor_move = [](const arcadia::input_cursor_move& e) -> bool
    {
        const auto& [wnd_ptr, move] = e.data_tuple;
        arcadia::log::debug(std::format("Move: {}, {}", move.x, move.y));
        return false;
    };
    ARCADIA_DISPATCH_EVENT(arcadia::input_cursor_pos, event, log_when_cursor_pos);
    ARCADIA_DISPATCH_EVENT(arcadia::input_cursor_move, event, log_when_cursor_move);
}
#endif // !NDEBUG
