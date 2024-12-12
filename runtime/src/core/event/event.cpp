#include "pch.hpp"

#include "event.hpp"

auto EventQueue::Instance() -> SelfType&
{
    static SelfType event_queue{};
    return event_queue;
}

auto EventQueue::SwapQueue() -> bool
{
    std::swap(_CurrentQueue, _ProcessingQueue);
    return GetSize();
}

auto EventQueue::GetSize() const -> size_t
{
    return _ProcessingQueue->size();
}

auto EventQueue::ReadFront() -> EventBase&
{
    if(!GetSize())
    {
        throw EmptyQueue{};
    }

    return *_ProcessingQueue->front();
}

auto EventQueue::PopFront() -> bool
{
    _ProcessingQueue->pop();
    return GetSize();
}