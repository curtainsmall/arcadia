#include "pch.hpp"

#include "app_context.hpp"

#ifdef ACDA_DEBUG_MODE
#   include"core/event/event.hpp"
#endif

auto AppContext::Instance() -> SelfType&
{
    static SelfType app_context{};
    return app_context;
}

AppContext::AppContext()
{
#ifdef ACDA_DEBUG_MODE
    auto& set = EventQueue::Instance()
        .DebugExcludedEventTypeIndexes;
    set.emplace(typeid(Events::InputCursorPos));
    set.emplace(typeid(Events::InputCursorMove));
    set.emplace(typeid(Events::WindowPosition));

#endif // ACDA_DEBUG_MODE
}