#include "pch.hpp"

#include "event.hpp"

auto Arcadia::EventQueue::Instance() -> SelfType&
{
    static SelfType event_queue{};
    return event_queue;
}

auto Arcadia::EventQueue::SwapQueue() -> bool
{
    std::swap(_CurrentQueue, _ProcessingQueue);
    return GetSize();
}

auto Arcadia::EventQueue::GetSize() const -> size_t
{
    return _ProcessingQueue->size();
}

auto Arcadia::EventQueue::GetFront() -> EventBase&
{
    if(!GetSize())
    {
        throw EmptyQueue{};
    }

    return *_ProcessingQueue->front();
}

auto Arcadia::EventQueue::PopFront() -> bool
{
    _ProcessingQueue->pop();
    return GetSize();
}