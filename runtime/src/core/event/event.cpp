#include "pch.hpp"

#include "event.hpp"

auto EventQueue::instance() -> self_type&
{
    static self_type event_queue{};
    return event_queue;
}

auto EventQueue::swap_queue() -> bool
{
    std::swap(_current_queue, _processing_queue);
    return size();
}

auto EventQueue::size() const -> std::size_t
{
    return _processing_queue->size();
}

auto EventQueue::read() -> EventBase&
{
    if(!size())
    {
        throw EmptyQueue{};
    }

    return *_processing_queue->front();
}

auto EventQueue::pop() -> bool
{
    _processing_queue->pop();
    return size();
}
