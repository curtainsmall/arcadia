#include "pch.hpp"

#include "event.hpp"

#include"core/assert.hpp"

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

auto Arcadia::EventQueue::GetSize() const -> std::size_t
{
    return _ProcessingQueue->size();
}

auto Arcadia::EventQueue::GetFront() -> EventBase&
{
    ACDA_ASSERT(GetSize(), "Empty event queue");
    return *_ProcessingQueue->front();
}

auto Arcadia::EventQueue::PopFront() -> bool
{
    _ProcessingQueue->pop();
    return GetSize();
}

void Arcadia::EventBase::MarkHandled()
{
    _Handled = true;
}

auto Arcadia::EventBase::IsHandled() const -> bool
{
    return _Handled;
}