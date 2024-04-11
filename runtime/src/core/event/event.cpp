#include "pch.hpp"
#include "event.hpp"

auto Arcadia::EventQueue::Instance() -> self_type&
{
    static self_type event_queue{};
    return event_queue;
}

auto Arcadia::EventQueue::SwapQueue() -> bool
{
    std::swap(_current_queue_ptr, _processing_queue_ptr);
    return Size();
}

auto Arcadia::EventQueue::Size() const -> std::size_t
{
    return _processing_queue_ptr->size();
}

auto Arcadia::EventQueue::Read() -> Arcadia::EventBase&
{
    if(!Size())
    {
        throw empty_queue{};
    }

    return *_processing_queue_ptr->front();
}

auto Arcadia::EventQueue::Pop() -> bool
{
    _processing_queue_ptr->pop();
    return Size();
}
