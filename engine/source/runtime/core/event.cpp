#include "core/event.hpp"

#include "core/assert.hpp"

auto Arcadia::EventQueue::Instance() -> SelfType&
{
    static SelfType event_queue{};
    return event_queue;
}

void Arcadia::EventQueue::SwapQueue()
{
    std::swap(_pCollectingQueue, _pProcessingQueue);
}

auto Arcadia::EventQueue::HasEvent() const -> bool
{
    return !_pProcessingQueue->empty();
}

auto Arcadia::EventQueue::ProcessEvent(const std::function<void(EventBase&)>& handler) -> bool
{
    ACDA_ASSERT(HasEvent());

    EventBase& event = *_pProcessingQueue->front();
    handler(event);
    return event.Handled();
}

void Arcadia::EventQueue::EventProcessFinished()
{
    _pProcessingQueue->pop();
}
