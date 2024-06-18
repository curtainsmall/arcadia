#include "pch.hpp"

#include "app_context.hpp"

#ifdef ARCADIA_IN_DEBUG
#   include"core/event/event.hpp"
#endif

auto AppContext::Instance() -> self_type&
{
    static self_type app_context{};
    return app_context;
}

AppContext::AppContext()
{
#ifdef ARCADIA_IN_DEBUG
    auto& set = EventQueue::Instance()
        .DebugExcludedEventTypes;
    set.emplace(typeid(Event::InputCursorPos));
    set.emplace(typeid(Event::InputCursorMove));
    set.emplace(typeid(Event::WindowPos));

#endif // ARCADIA_IN_DEBUG

}
