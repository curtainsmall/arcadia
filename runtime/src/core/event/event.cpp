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
    return size();
}

auto arcadia::event_queue::size() -> std::size_t
{
    return _processing_queue_ptr->size();
}

auto arcadia::event_queue::read() -> arcadia::event&
{
    if(!size())
    {
        throw empty_queue{};
    }

    return *_processing_queue_ptr->front();
}

auto arcadia::event_queue::pop() -> bool
{
    _processing_queue_ptr->pop();
    return size();
}
