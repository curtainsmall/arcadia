#pragma once

#include<functional>
#include<memory>
#include<queue>
#include<tuple>
#include<typeindex>
#include<typeinfo>
#include<unordered_set>
#include<vector>

#include"core/log/log.hpp"
#include"core/noncopyable.hpp"
#include"platform/api_def.hpp"
#include"platform/debug_def.hpp"

#ifdef ACDA_DEBUG_MODE
#   include<unordered_set>
#   include<typeindex>
#endif // ACDA_DEBUG_MODE

namespace Arcadia
{
    class EventBase: public Noncopyable
    {
    public:
        EventBase() = default;
        // Virtual destructor that make event type virtual
        virtual ~EventBase()
        {};

        void MarkHandled();

        auto IsHandled() const -> bool;
    private:
        bool _Handled{ false };
    };

    namespace Concepts
    {
        template<typename T>
        concept Event = std::derived_from<T, EventBase>;
    }

    template<typename ...Args>
    class BasicEvent: public EventBase
    {
    public:
        using DataTupleType = std::tuple<Args...>;

        using SelfType = BasicEvent<Args...>;
    public:
        BasicEvent(Args ...args) :
            DataTuple(std::make_tuple<Args...>(std::forward<Args>(args)...))
        {}
        virtual ~BasicEvent() = default;

        operator const DataTupleType() const
        {
            return DataTuple;
        }

    public:
        const DataTupleType DataTuple;
    };

    template<Concepts::Event Event>
    using EventHandler = std::function<void(Event&)>;

    class EventDispatcher: public Noncopyable
    {
    public:
        using SelfType = EventDispatcher;
    public:
        EventDispatcher(EventBase& event) :
            _Event(&event)
        {}
        ~EventDispatcher() = default;

        template<Concepts::Event Event>
        auto Dispatch(const EventHandler<Event>& handler) -> SelfType&
        {
            if(typeid(*_Event) == typeid(Event))
            {
                handler(static_cast<Event&>(*_Event));
                _Dispatched = true;
            }
            return *this;
        }

        auto IsDispatched() const -> bool
        {
            return _Dispatched;
        }

    private:
        EventBase* _Event;
        bool _Dispatched{ false };
    };

    class EventQueue
    {
    public:

        using SelfType = EventQueue;
        using DebugExcludedEventTypeSetType = std::unordered_set<std::type_index>;
    private:
        using _EventQueueType = std::queue<std::unique_ptr<EventBase>>;

    public:
        static auto Instance() -> SelfType&;

        template<Concepts::Event Event, typename ...Args>
        auto Signal(Args&& ...args) -> SelfType&
        {
            _CurrentQueue->emplace(std::make_unique<Event>(std::forward<Args>(args)...));

#ifdef ACDA_DEBUG_MODE
            if(!DebugExcludedEventTypeSet.contains(typeid(Event)))
            {
                ACDA_LOG_DEBUG(std::format("Event signaled: {}", typeid(Event).name()));
            }
#endif
            return *this;
        }

        auto SwapQueue() -> bool;

        auto GetSize() const->std::size_t;

        auto GetFront() -> EventBase&;

        auto PopFront() -> bool;

    public:
#ifdef ACDA_DEBUG_MODE
        DebugExcludedEventTypeSetType DebugExcludedEventTypeSet{};
#endif // ACDA_DEBUG_MODE

    private:
        _EventQueueType _QueueA{};
        _EventQueueType _QueueB{};
        _EventQueueType* _ProcessingQueue{ &_QueueA };
        _EventQueueType* _CurrentQueue{ &_QueueB };
    };
}