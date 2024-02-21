#pragma once

#include<functional>
#include<memory>
#include<queue>
#include<tuple>
#include<typeinfo>
#include<vector>

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/util/string.hpp"
#include"core/util/template.hpp"

#define ARCADIA_EVENT(event_name,...) \
struct ARCADIA_API event_name: arcadia::basic_event<__VA_ARGS__>\
{\
public:\
    using arcadia::basic_event<__VA_ARGS__>::basic_event;\
}

#define ARCADIA_DISPATCH_EVENT(event_type, event_instance, event_handler) \
arcadia::dispatch_event<event_type>(\
    event_instance,\
    [&](const event_type& evt) -> bool\
{\
    return event_handler(evt);\
}\
)

namespace arcadia
{
    struct ARCADIA_API event
    {
    public:
        virtual constexpr auto get_type_info() const -> const std::type_info & = 0;

    };

    template<class Event>
    concept event_like = requires{
        Event::get_type_info_static();
        std::derived_from<Event, arcadia::event>;
    };

    template<class ...Args>
    struct ARCADIA_API basic_event: arcadia::event
    {
    public:
        using data_tuple_type = std::tuple<Args...>;

        using self_type = basic_event<Args...>;
    public:

        static constexpr auto get_type_info_static() -> const std::type_info&
        {
            return typeid(self_type);
        }

        virtual constexpr auto get_type_info() const -> const std::type_info & override
        {
            return get_type_info_static();
        }

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
    /// @param fn Handler
    /// @return Return value of handler: @b true for keep event signaled or @b false for skip remaining handlers; 
    /// or returns @b true if @a event is not type of @a Event (handler is not called)
    template<arcadia::event_like Event>
    ARCADIA_API auto dispatch_event(
        const arcadia::event& event,
        const arcadia::event_handler<Event>& fn
    ) -> bool
    {
        if(typeid(event) == typeid(Event))
        {
            return fn(static_cast<const Event&>(event));
        }
        return true;
    }

    struct ARCADIA_API event_queue
    {
    public:
        ARCADIA_EXCEPTION(empty_queue);

        using self_type = event_queue;
    private:
        using _event_uptr_queue_type = std::queue<std::unique_ptr<arcadia::event>>;

    public:
        static auto instance() -> self_type&;

        /// @brief Signal @a Event
        /// @param ...args Argument to construct @a Event
        template<arcadia::event_like Event, class ...Args>
        void signal(Args&& ...args)
        {
            _current_queue_ptr->emplace(std::make_unique<Event>(std::forward<Args>(args)...));
        }

        /// @brief Swap current queue and processing queue
        /// @return whether the processing queue contains event after swap
        auto swap_queue() -> bool;

        /// @brief Check whther the proceessing queue contains event
        auto size() -> std::size_t;

        /// @brief Read the front event in event queue
        /// @return Event at front
        auto read() -> arcadia::event&;

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