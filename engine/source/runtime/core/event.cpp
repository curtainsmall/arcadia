#include "pch.hpp"

#include "event.hpp"

#include "core/assert.hpp"

auto Arcadia::EventQueue::Instance() -> SelfType&
{
    static SelfType event_queue {};
    return event_queue;
}

void Arcadia::EventQueue::SwapQueue()
{
    std::swap(_pCollectingQueue, _pProcessingQueue);
}

auto Arcadia::EventQueue::HasEvent() const -> bool
{
    return _pProcessingQueue->size();
}

auto Arcadia::EventQueue::ProcessEvent(const EventHandler<EventBase>& handler) -> bool
{
    ACDA_ASSERT(HasEvent());

    EventBase& event = *_pProcessingQueue->front();
    handler(event);
    return !!(event.GetHandleState() & EventHandleState::Handled);
}

void Arcadia::EventQueue::EventProcessFinished()
{
    std::unique_ptr<EventBase> event_uptr = std::move(_pProcessingQueue->front());
    _pProcessingQueue->pop();
    switch(event_uptr->GetHandleState())
    {
        case EventHandleState::OnceAgain:
        {
            event_uptr->ClearMark();
            _pCollectingQueue->push(std::move(event_uptr));
            break;
        }
        default:
        {
            break;
        }
    }
}

void Arcadia::EventBase::MarkHandled()
{
    _HandleState |= EventHandleState::Handled;
}

void Arcadia::EventBase::MarkOnceAgain()
{
    _HandleState |= EventHandleState::OnceAgain;
}

auto Arcadia::EventBase::GetHandleState() const -> EventHandleState
{
    return _HandleState;
}

void Arcadia::EventBase::ClearMark()
{
    _HandleState = EventHandleState::NotHandled;
}

auto Arcadia::EventDispatcher::IsDispatched() const -> bool
{
    return _Dispatched;
}