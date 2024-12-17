#include "pch.hpp"

#include "app_context.hpp"

#include"platform/debug_def.hpp"

#ifdef ACDA_DEBUG_MODE
#   include"core/event/event.hpp"
#endif

auto Arcadia::AppContext::Instance() -> SelfType&
{
    static SelfType app_context{};
    return app_context;
}

Arcadia::AppContext::AppContext()
{
#ifdef ACDA_DEBUG_MODE
    EventQueue::DebugExcludedEventTypeSetType set =
        EventQueue::Instance().DebugExcludedEventTypeSet;
    set.emplace(typeid(Events::InputCursorPos));
    set.emplace(typeid(Events::InputCursorMove));
    set.emplace(typeid(Events::WindowSetPosition));

#endif // ACDA_DEBUG_MODE
}