#include "pch.hpp"

#include "event.hpp"

#include "core/assert.hpp"

auto Arcadia::EventQueue::Instance() -> SelfType&
{
    static SelfType event_queue{};
    return event_queue;
}

auto Arcadia::EventQueue::SwapQueue() -> bool
{
    std::swap(_pCurrentQueue, _pProcessingQueue);
    return GetSize();
}

auto Arcadia::EventQueue::GetSize() const -> std::size_t
{
    return _pProcessingQueue->size();
}

auto Arcadia::EventQueue::GetFront() -> EventBase&
{
    ACDA_ASSERT(GetSize(), "Empty event queue");
    return *_pProcessingQueue->front();
}

auto Arcadia::EventQueue::PopFront() -> bool
{
    _pProcessingQueue->pop();
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

auto Arcadia::EventDispatcher::IsDispatched() const -> bool
{
    return _Dispatched;
}
