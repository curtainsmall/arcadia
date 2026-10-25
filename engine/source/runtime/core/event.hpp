#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <queue>
#include <type_traits>
#include <typeinfo>
#include <utility>

#include "foundation/event_bus.hpp"
#include "platform/defines.hpp"

// Core event layer.
//
// The event base and the bus now come from the foundation ring: every event in
// the engine derives from Foundation::Event, and the "consumed" flag lives on
// that base instead of in a per-module state enum.
//
// What is left here is the legacy *shape* of the API that the runtime modules and
// the editor still use (typeid chain + queued broadcast). It is a thin adapter:
// once the last module is moved onto EventBus::Subscribe / SubscribeAny, this
// file disappears.

namespace Arcadia
{
    using EventBase = Foundation::Event;

    namespace Concepts
    {
        template<class T>
        concept Event = std::derived_from<T, EventBase> && (not std::is_abstract_v<T>);
    }

    template<Concepts::Event E>
    using EventHandler = std::function<void(E&)>;

    // Chainable "does this event have type E?" dispatcher. Equivalent to a
    // typeid compare; kept so the existing OnEvent bodies stay untouched.
    struct ACDA_API EventDispatcher
    {
    public:
        explicit EventDispatcher(EventBase& event):
            _pEvent(&event)
        {
        }

        EventDispatcher(const EventDispatcher&) = delete;
        auto operator=(const EventDispatcher&) -> EventDispatcher& = delete;

        template<Concepts::Event E>
        auto Dispatch(const EventHandler<E>& handler) -> EventDispatcher&
        {
            if(!_halt && typeid(*_pEvent) == typeid(E))
            {
                handler(static_cast<E&>(*_pEvent));
                _dispatched = true;
            }
            return *this;
        }

        auto HaltDispatchIf(bool condition = true) -> EventDispatcher&
        {
            if(condition)
            {
                _halt = true;
            }
            return *this;
        }

        // The legacy OnEvent bodies end their chain with a discarded IsDispatched(),
        // so the result is deliberately not marked [[nodiscard]].
        auto IsDispatched() const -> bool
        {
            return _dispatched;
        }

    private:
        EventBase* _pEvent{ nullptr };
        bool _dispatched{ false };
        bool _halt{ false };
    };

    // Frame-deferred broadcast queue: Signal() enqueues, the main loop swaps the
    // queues once per frame and hands each event to the subscribers in order.
    //
    // Engine owns the EventBus that the frame loop drives; this queue is kept for
    // the call sites that still use its explicit swap/process API.
    struct ACDA_API EventQueue
    {
    public:
        using QueueType = std::queue<std::unique_ptr<EventBase>>;
        using SelfType = EventQueue;

    public:
        static auto Instance() -> SelfType&;

        EventQueue(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType& = delete;

        template<Concepts::Event E, class ...Args>
        auto Signal(Args&& ...args) -> SelfType&
        {
            _pCollectingQueue->emplace(std::make_unique<E>(std::forward<Args>(args)...));
            return *this;
        }

        void SwapQueue();
        [[nodiscard]] auto HasEvent() const -> bool;

        // Returns true when the event was consumed (Event::MarkHandled()).
        auto ProcessEvent(const std::function<void(EventBase&)>& handler) -> bool;
        void EventProcessFinished();

    private:
        EventQueue() = default;
        ~EventQueue() = default;

    private:
        QueueType _QueueA{};
        QueueType _QueueB{};
        QueueType* _pProcessingQueue{ &_QueueA };
        QueueType* _pCollectingQueue{ &_QueueB };
    };
}
