#include "pch.hpp"
#include "event.hpp"

auto arcadia::event_queue::instance() -> self_type&
{
    static self_type event_queue{};
    return event_queue;
}

auto arcadia::event_queue::swap_queue() -> bool
{
    std::swap(_current_queue_ptr, _processing_queue_ptr);
    return non_empty();
}

auto arcadia::event_queue::non_empty() -> bool
{
    return !_processing_queue_ptr->empty();
}

auto arcadia::event_queue::read() -> arcadia::event&
{
    if(!non_empty())
    {
        throw empty_queue{};
    }

    return *_processing_queue_ptr->front();
}

auto arcadia::event_queue::pop() -> bool
{
    _processing_queue_ptr->pop();
    return non_empty();
}
