#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <unordered_set>
#include <vector>

#include "core/log.hpp"
#include "core/noncopyable.hpp"
#include "platform/api_def.hpp"
#include "platform/debug_def.hpp"

namespace Arcadia
{
    struct EventBase: public Noncopyable
    {
    public:
        EventBase() = default;
        // Virtual destructor that make event type virtual
        virtual ~EventBase() = default;

        void MarkHandled();
        auto IsHandled() const -> bool;

    private:
        bool _Handled{ false };
    };

    namespace Concepts
    {
        template<class T>
        concept Event = std::derived_from<T, EventBase>;
    }

    template<Concepts::Event Event>
    using EventHandler = std::function<void(Event&)>;

    struct EventDispatcher: public Noncopyable
    {
    public:
        using SelfType = EventDispatcher;
    public:
        EventDispatcher(EventBase& event):
            _pEvent(&event)
        {
        }
        ~EventDispatcher() = default;

        template<Concepts::Event Event>
        auto Dispatch(const EventHandler<Event>& handler) -> SelfType&
        {
            if(typeid(*_pEvent) == typeid(Event))
            {
                handler(static_cast<Event&>(*_pEvent));
                _Dispatched = true;
            }
            return *this;
        }

        auto IsDispatched() const -> bool;

    private:
        EventBase* _pEvent;
        bool _Dispatched{ false };
    };

    struct EventQueue
    {
    public:
        using SelfType = EventQueue;
        using DebugExcludedEventTypeSetType = std::unordered_set<std::type_index>;
    private:
        using _EventQueueType = std::queue<std::unique_ptr<EventBase>>;

    public:
        static auto Instance() -> SelfType&;

        template<Concepts::Event Event, class ...Args>
        auto Signal(Args&& ...args) -> SelfType&
        {
            _pCurrentQueue->emplace(std::make_unique<Event>(std::forward<Args>(args)...));

            return *this;
        }

        auto SwapQueue() -> bool;

        auto GetSize() const->std::size_t;

        auto GetFront() -> EventBase&;

        auto PopFront() -> bool;

    private:
        _EventQueueType _QueueA{};
        _EventQueueType _QueueB{};
        _EventQueueType* _pProcessingQueue{ &_QueueA };
        _EventQueueType* _pCurrentQueue{ &_QueueB };
    };
}