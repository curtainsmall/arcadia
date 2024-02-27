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
#include"core/util/string.hpp"
#include"core/util/template.hpp"

#define ARCADIA_EVENT(event_name,...) \
struct ARCADIA_API event_name: arcadia::basic_event<__VA_ARGS__>\
{\
public:\
    using arcadia::basic_event<__VA_ARGS__>::basic_event;\
}\


#define ARCADIA_DISPATCH_EVENT(event_type, event_instance, event_handler) \
arcadia::dispatch_event<event_type>(\
    event_instance,\
    [&](const event_type& evt) -> bool\
{\
    return event_handler(evt);\
}\
)

#ifndef NDEBUG
#   define ARCADIA_DEBUG_SHOULD_LOG_EVENT_INFO_WHEN_SIGNALED 1
#endif


namespace arcadia
{
    struct ARCADIA_API event_base
    {
        // Virtual destructor that make event type virtual
        virtual ~event_base()
        {};
    };

    template<class Event>
    concept event_like = requires{
        std::derived_from<Event, arcadia::event_base>;
    };

    template<class ...Args>
    struct ARCADIA_API basic_event: arcadia::event_base
    {
    public:
        using data_tuple_type = std::tuple<Args...>;

        using self_type = basic_event<Args...>;
    public:
        /// @brief Construct a signaled event
        basic_event(Args ...args):
            data_tuple(std::make_tuple<Args...>(std::forward<Args>(args)...))
        {}
        virtual ~basic_event() = default;

    public:
        data_tuple_type data_tuple;

    };

    template<arcadia::event_like Event>
    using event_handler = std::function<bool(const Event&)>;

    /// @brief Dispatch @a event to handler that receives @a Event
    /// @param event Event to dispatch
    /// @param handler Handler
    /// @return 
    /// @b true if the handler returns true (meaning this event is handled and should not be propagate further), or
    /// @b false if ther handler returns false or if the event type does not match
    template<arcadia::event_like Event>
    ARCADIA_API auto dispatch_event(
        const arcadia::event_base& event,
        const arcadia::event_handler<Event>& handler
    ) -> bool
    {
        if(typeid(event) == typeid(Event))
        {
            return handler(static_cast<const Event&>(event));
        }
        return false;
    }

    struct ARCADIA_API event_dispatcher
    {
    public:
        ARCADIA_EXCEPTION(process_truncated);

        using self_type = event_dispatcher;
    public:
        event_dispatcher(const arcadia::event_base& event):
            _event_ptr(&event)
        {}
        ~event_dispatcher() = default;

        /// @brief Dispatch stored event to handler that receives @a Event
        /// @param handler Handler
        /// @return 
        /// @b true if the handler returns true (meaning this event is handled and should not be propagate further), or
        /// @b false if ther handler returns false or if the event type does not match
        template<arcadia::event_like Event>
        auto dispatch(const arcadia::event_handler<Event>& handler) -> bool
        {
            if(typeid(*_event_ptr) == typeid(Event))
            {
                return handler(static_cast<const Event&>(*_event_ptr));
            }
            return false;
        }

        /// @brief Dispatch stored evetn to handler that receives @a Event
        /// @param handler Handler
        /// @return This event dispatcher (when the handler returns false or when this event type does not match)
        /// @throw process_truncated if the handler returns true (meaning this event is handled and should not be propagate further)
        template<arcadia::event_like Event>
        auto dispatch_chained(const arcadia::event_handler<Event>& handler) -> self_type&
        {
            if(typeid(*_event_ptr) == typeid(Event))
            {
                if(handler(static_cast<const Event&>(*_event_ptr)))
                {
                    throw process_truncated{};
                }
            }
            return *this;
        }
    private:
        const arcadia::event_base* _event_ptr;
    };

    struct ARCADIA_API event_queue
    {
    public:
        ARCADIA_EXCEPTION(empty_queue);

        using self_type = event_queue;
    private:
        using _event_uptr_queue_type = std::queue<std::unique_ptr<arcadia::event_base>>;

    public:
        static auto instance() -> self_type&;

        /// @brief Signal @a Event
        /// @param ...args Argument to construct @a Event
        template<arcadia::event_like Event, class ...Args>
        void signal(Args&& ...args)
        {
            _current_queue_ptr->emplace(std::make_unique<Event>(std::forward<Args>(args)...));

        #if ARCADIA_DEBUG_SHOULD_LOG_EVENT_INFO_WHEN_SIGNALED
            arcadia::log::debug(std::format("Event signaled: {}", typeid(Event).name()));
        #endif
        }

        /// @brief Swap current queue and processing queue
        /// @return whether the processing queue contains event after swap
        auto swap_queue() -> bool;

        /// @brief Check whther the proceessing queue contains event
        auto size() -> std::size_t;

        /// @brief Read the front event in event queue
        /// @return Event at front
        auto read() -> arcadia::event_base&;

        /// @brief Pop front event
        /// @return whether the processing queue contains event after pop;
        auto pop() -> bool;

    private:
        _event_uptr_queue_type _queue_a{};
        _event_uptr_queue_type _queue_b{};
        _event_uptr_queue_type* _processing_queue_ptr{ &_queue_a };
        _event_uptr_queue_type* _current_queue_ptr{ &_queue_b };
    };

}