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

#ifdef ACDA_DEBUG_MODE
#   include<unordered_set>
#   include<typeindex>
#endif // ACDA_DEBUG_MODE

#define ACDA_EVENT(event_name,...) \
struct event_name: BasicEvent<__VA_ARGS__>\
{\
public:\
    using self_type = event_name;\
public:\
    using BasicEvent<__VA_ARGS__>::BasicEvent;\
}

struct EventBase: Noncopyable
{
public:
    EventBase() = default;
    // Virtual destructor that make event type virtual
    virtual ~EventBase()
    {};
public:
    bool handled{ false };
};

template<class Event>
concept cEvent = requires{
    std::derived_from<Event, EventBase>;
};

template<class ...Args>
struct BasicEvent: EventBase
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

template<cEvent Event>
using EventHandler = std::function<void(Event&)>;

struct EventDispatcher: Noncopyable
{
public:
    using self_type = EventDispatcher;
public:
    EventDispatcher(EventBase& event):
        _event(&event)
    {}
    ~EventDispatcher() = default;

    /// @brief Dispatch stored event to given handler. If their types match, the handler will be excuted at once
    /// @tparam Event Event type to match
    /// @param handler Event handler
    /// @return Self
    template<cEvent Event>
    auto dispatch(const EventHandler<Event>& handler) -> self_type&
    {
        if(typeid(*_event) == typeid(Event))
        {
            handler(static_cast<Event&>(*_event));
            _result = true;
        }
        return *this;
    }

    /// @brief Whether any dispatch succedded
    auto result() const -> bool
    {
        return _result;
    }

private:
    EventBase* _event;
    bool _result{ false };
};

struct EventQueue
{
public:
    ACDA_EXCEPTION(EmptyQueue);

    using self_type = EventQueue;
private:
    using _event_queue_type = std::queue<std::unique_ptr<EventBase>>;

public:
    static auto instance() -> self_type&;

    /// @brief Signal @a Event
    /// @param ...args Argument to construct @a Event
    template<cEvent event, class ...Args>
    auto signal(Args&& ...args) -> self_type&
    {
        _current_queue->emplace(std::make_unique<event>(std::forward<Args>(args)...));

    #ifdef ACDA_DEBUG_MODE
        if(!debug_excluded_event_types.contains(typeid(event)))
        {
            LOG_DEBUG(std::format("Event signaled: {}", typeid(event).name()));
        }
    #endif
        return *this;
    }

    /// @brief Swap current queue and processing queue
    /// @return whether the processing queue contains event after swap
    auto swap_queue() -> bool;

    /// @brief Check whther the proceessing queue contains event
    auto size() const->size_t;

    /// @brief Read the front event in event queue
    /// @return Event at front
    auto read() -> EventBase&;

    /// @brief Pop front event
    /// @return whether the processing queue contains event after pop;
    auto pop() -> bool;

public:
#ifdef ACDA_DEBUG_MODE
    std::unordered_set<std::type_index> debug_excluded_event_types{};
#endif // ACDA_DEBUG_MODE

private:
    _event_queue_type _queue_a{};
    _event_queue_type _queue_b{};
    _event_queue_type* _processing_queue{ &_queue_a };
    _event_queue_type* _current_queue{ &_queue_b };
};
