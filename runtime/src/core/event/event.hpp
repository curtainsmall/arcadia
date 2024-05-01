#pragma once

#include<functional>
#include<memory>
#include<queue>
#include<tuple>
#include<typeinfo>
#include<vector>

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/log/log.hpp"

#ifdef ARCADIA_IN_DEBUG
#   include<unordered_set>
#   include<typeindex>
#endif // ARCADIA_IN_DEBUG

#define ARCADIA_EVENT(event_name,...) \
struct event_name: Arcadia::BasicEvent<__VA_ARGS__>\
{\
public:\
    using self_type = event_name;\
public:\
    using Arcadia::BasicEvent<__VA_ARGS__>::BasicEvent;\
}

namespace Arcadia
{
    struct EventBase: Arcadia::Noncopyable
    {
    public:
        EventBase() = default;
        // Virtual destructor that make event type virtual
        virtual ~EventBase()
        {};
    public:
        bool Handled{ false };
    };

    template<class Event>
    concept cEvent = requires{
        std::derived_from<Event, Arcadia::EventBase>;
    };

    template<class ...Args>
    struct BasicEvent: Arcadia::EventBase
    {
    public:
        using data_tuple_type = std::tuple<Args...>;

        using self_type = BasicEvent<Args...>;
    public:
        /// @brief Construct a signaled event
        BasicEvent(Args ...args):
            data_tuple(std::make_tuple<Args...>(std::forward<Args>(args)...))
        {}
        virtual ~BasicEvent() = default;

        operator const data_tuple_type() const
        {
            return data_tuple;
        }

    public:
        const data_tuple_type data_tuple;
    };

    template<Arcadia::cEvent Event>
    using EventHandler = std::function<void(Event&)>;

    struct EventDispatcher: Arcadia::Noncopyable
    {
    public:
        using self_type = EventDispatcher;
    public:
        EventDispatcher(Arcadia::EventBase& event):
            _pEvent(&event)
        {}
        ~EventDispatcher() = default;

        /// @brief Dispatch stored event to given handler. If their types match, the handler will be excuted at once
        /// @tparam Event Event type to match
        /// @param handler Event handler
        /// @return Self
        template<Arcadia::cEvent Event>
        auto Dispatch(const Arcadia::EventHandler<Event>& handler) -> self_type&
        {
            if(typeid(*_pEvent) == typeid(Event))
            {
                handler(static_cast<Event&>(*_pEvent));
                _Result = true;
            }
            return *this;
        }

        /// @brief Whether any dispatch succedded
        auto Result() const -> bool
        {
            return _Result;
        }

    private:
        Arcadia::EventBase* _pEvent;
        bool _Result{ false };
    };

    struct EventQueue
    {
    public:
        ARCADIA_EXCEPTION(empty_queue);

        using self_type = EventQueue;
    private:
        using _event_queue_type = std::queue<std::unique_ptr<Arcadia::EventBase>>;

    public:
        static auto Instance() -> self_type&;

        /// @brief Signal @a Event
        /// @param ...args Argument to construct @a Event
        template<Arcadia::cEvent Event, class ...Args>
        auto Signal(Args&& ...args) -> self_type&
        {
            _CurrentQueue->emplace(std::make_unique<Event>(std::forward<Args>(args)...));

        #ifdef ARCADIA_IN_DEBUG
            if(!DebugExcludedEventTypes.contains(typeid(Event)))
            {
                Arcadia::Log::Debug(std::format("Event signaled: {}", typeid(Event).name()));
            }
        #endif
            return *this;
        }

        /// @brief Swap current queue and processing queue
        /// @return whether the processing queue contains event after swap
        auto SwapQueue() -> bool;

        /// @brief Check whther the proceessing queue contains event
        auto Size() const->std::size_t;

        /// @brief Read the front event in event queue
        /// @return Event at front
        auto Read() -> Arcadia::EventBase&;

        /// @brief Pop front event
        /// @return whether the processing queue contains event after pop;
        auto Pop() -> bool;

    public:
    #ifdef ARCADIA_IN_DEBUG
        std::unordered_set<std::type_index> DebugExcludedEventTypes{};
    #endif // ARCADIA_IN_DEBUG

    private:
        _event_queue_type _QueueA{};
        _event_queue_type _QueueB{};
        _event_queue_type* _ProcessingQueue{ &_QueueA };
        _event_queue_type* _CurrentQueue{ &_QueueB };
    };

}

