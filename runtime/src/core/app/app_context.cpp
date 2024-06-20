#include "pch.hpp"

#include "app_context.hpp"

#ifdef ACDA_DEBUG_MODE
#   include"core/event/event.hpp"
#endif

auto AppContext::instance() -> self_type&
{
    static self_type app_context{};
    return app_context;
}

AppContext::AppContext()
{
#ifdef ACDA_DEBUG_MODE
    auto& set = EventQueue::instance()
        .debug_excluded_event_types;
    set.emplace(typeid(event::InputCursorPos));
    set.emplace(typeid(event::InputCursorMove));
    set.emplace(typeid(event::window_pos));

#endif // ACDA_DEBUG_MODE

}
