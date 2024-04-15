#include "pch.hpp"
#include "event.hpp"

auto Arcadia::EventQueue::Instance() -> self_type&
{
    static self_type event_queue{};
    return event_queue;
}

auto Arcadia::EventQueue::SwapQueue() -> bool
{
    std::swap(_CurrentQueue, _ProcessingQueue);
    return Size();
}

auto Arcadia::EventQueue::Size() const -> std::size_t
{
    return _ProcessingQueue->size();
}

auto Arcadia::EventQueue::Read() -> Arcadia::EventBase&
{
    if(!Size())
    {
        throw empty_queue{};
    }

    return *_ProcessingQueue->front();
}

auto Arcadia::EventQueue::Pop() -> bool
{
    _ProcessingQueue->pop();
    return Size();
}
