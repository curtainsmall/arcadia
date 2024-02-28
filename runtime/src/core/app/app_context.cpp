#include "pch.hpp"
#include "app_context.hpp"

#ifdef ARCADIA_IN_DEBUG
#   include"core/event/event.hpp"
#endif

auto arcadia::app_context::instance() -> self_type&
{
    static self_type app_context{};
    return app_context;
}

arcadia::app_context::app_context()
{
#ifdef ARCADIA_IN_DEBUG
    auto& set = arcadia::event_queue::instance()
        .debug_excluded_event_type_set;
    set.emplace(typeid(arcadia::event::input_cursor_pos));
    set.emplace(typeid(arcadia::event::input_cursor_move));

#endif // ARCADIA_IN_DEBUG

}
