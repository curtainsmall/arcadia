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
    set.emplace(typeid(events::InputCursorPos));
    set.emplace(typeid(events::InputCursorMove));
    set.emplace(typeid(events::window_pos));

#endif // ACDA_DEBUG_MODE

}
