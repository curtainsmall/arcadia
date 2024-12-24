#pragma once

#include<functional>
#include<memory>
#include<queue>
#include<tuple>
#include<typeindex>
#include<typeinfo>
#include<unordered_set>
#include<vector>

#include"core/exception.hpp"
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

    template<typename Event>
    concept cEvent = std::derived_from<Event, EventBase>;

    template<typename ...Args>
    class BasicEvent: public EventBase
    {
    public:
        using DataTupleType = std::tuple<Args...>;

        using SelfType = BasicEvent<Args...>;
    public:
        /// @brief Conclass a signaled event
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

    template<cEvent Event>
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

        /// @brief Dispatch stored event to given handler. If their types match, the handler will be excuted at once
        /// @tparam Event Event type to match
        /// @param handler Event handler
        /// @return Self
        template<cEvent Event>
        auto Dispatch(const EventHandler<Event>& handler) -> SelfType&
        {
            if(typeid(*_Event) == typeid(Event))
            {
                handler(static_cast<Event&>(*_Event));
                _Dispatched = true;
            }
            return *this;
        }

        /// @brief Whether any dispatch succedded
        auto IsDispatched() const -> bool
        {
            return _Dispatched;
        }

    private:
        EventBase* _Event;
        bool _Dispatched{ false };
    };

    namespace Exceptions
    {
        ACDA_DEFINE_EXCEPTION(EmptyEventQueue);
    }

    class EventQueue
    {
    public:

        using SelfType = EventQueue;
        using DebugExcludedEventTypeSetType = std::unordered_set<std::type_index>;
    private:
        using _EventQueueType = std::queue<std::unique_ptr<EventBase>>;

    public:
        static auto Instance() -> SelfType&;

        /// @brief Signal @a Event
        /// @param ...args Argument to construct @a Event
        template<cEvent Event, typename ...Args>
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

        /// @brief Swap current queue and processing queue
        /// @return whether the processing queue contains event after swap
        auto SwapQueue() -> bool;

        /// @brief Check whther the proceessing queue contains event
        auto GetSize() const->std::size_t;

        /// @brief Read the front event in event queue
        /// @return Event at front
        auto GetFront() -> EventBase&;

        /// @brief Pop front event
        /// @return whether the processing queue contains event after pop;
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