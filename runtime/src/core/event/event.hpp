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
struct ARCADIA_API event_name: arcadia::basic_event<__VA_ARGS__>\
{\
public:\
    using self_type = event_name;\
public:\
    using arcadia::basic_event<__VA_ARGS__>::basic_event;\
}

namespace arcadia
{
    struct ARCADIA_API event_base: arcadia::noncopyable
    {
    public:
        event_base() = default;
        // Virtual destructor that make event type virtual
        virtual ~event_base()
        {};
    public:
        bool handled{ false };
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

        operator const data_tuple_type() const
        {
            return data_tuple;
        }

        template<std::size_t Index>
        auto get() const -> const auto&
        {
            static_assert(Index < std::tuple_size_v<data_tuple_type>);
            return std::get<Index>(data_tuple);
        }

    public:
        const data_tuple_type data_tuple;

    };

    template<arcadia::event_like Event>
    using event_handler = std::function<void(Event&)>;

    /// @brief Dispatch @a event to handler that receives @a Event
    /// @param event Event to dispatch
    /// @param handler Handler
    /// @return 
    /// - @b true if successfully dispatched
    /// - @b false if the event type does not match
    template<arcadia::event_like Event>
    ARCADIA_API auto dispatch_event(
        arcadia::event_base& event,
        const arcadia::event_handler<Event>& handler
    ) -> bool
    {
        if(typeid(event) == typeid(Event))
        {
            handler(static_cast<Event&>(event));
            return true;
        }
        return false;
    }

    struct ARCADIA_API event_dispatcher: arcadia::noncopyable
    {
    public:
        using self_type = event_dispatcher;
    public:
        inline event_dispatcher(arcadia::event_base& event):
            _event_ptr(&event)
        {}
        ~event_dispatcher() = default;

        /// @brief Dispatch stored event to given handler. If their types match, the handler will be excuted at once
        /// @tparam Event Event type to match
        /// @param handler Event handler
        /// @return Self
        template<arcadia::event_like Event>
        auto dispatch(const arcadia::event_handler<Event>& handler) -> self_type&
        {
            if(typeid(*_event_ptr) == typeid(Event))
            {
                handler(static_cast<Event&>(*_event_ptr));
                _result = true;
            }
            return *this;
        }

        /// @brief Whether any dispatch succedded
        inline auto result() const -> bool
        {
            return _result;
        }

    private:
        arcadia::event_base* _event_ptr;
        bool _result{ false };
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
        auto signal(Args&& ...args) -> self_type&
        {
            _current_queue_ptr->emplace(std::make_unique<Event>(std::forward<Args>(args)...));

        #ifdef ARCADIA_IN_DEBUG
            if(!debug_excluded_event_type_set.contains(typeid(Event)))
            {
                arcadia::log::debug(std::format("Event signaled: {}", typeid(Event).name()));
            }
        #endif
            return *this;
        }

        /// @brief Swap current queue and processing queue
        /// @return whether the processing queue contains event after swap
        auto swap_queue() -> bool;

        /// @brief Check whther the proceessing queue contains event
        auto size() const->std::size_t;

        /// @brief Read the front event in event queue
        /// @return Event at front
        auto read() -> arcadia::event_base&;

        /// @brief Pop front event
        /// @return whether the processing queue contains event after pop;
        auto pop() -> bool;

    public:
    #ifdef ARCADIA_IN_DEBUG
        std::unordered_set<std::type_index> debug_excluded_event_type_set{};
    #endif // ARCADIA_IN_DEBUG

    private:
        _event_uptr_queue_type _queue_a{};
        _event_uptr_queue_type _queue_b{};
        _event_uptr_queue_type* _processing_queue_ptr{ &_queue_a };
        _event_uptr_queue_type* _current_queue_ptr{ &_queue_b };
    };

}

