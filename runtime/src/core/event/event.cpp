#include "pch.hpp"

#include "event.hpp"

auto EventQueue::Instance() -> self_type&
{
    static self_type event_queue{};
    return event_queue;
}

auto EventQueue::SwapQueue() -> bool
{
    std::swap(_CurrentQueue, _ProcessingQueue);
    return Size();
}

auto EventQueue::Size() const -> std::size_t
{
    return _ProcessingQueue->size();
}

auto EventQueue::Read() -> EventBase&
{
    if(!Size())
    {
        throw empty_queue{};
    }

    return *_ProcessingQueue->front();
}

auto EventQueue::Pop() -> bool
{
    _ProcessingQueue->pop();
    return Size();
}
