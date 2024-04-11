#include "pch.hpp"
#include "app_context.hpp"

#ifdef ARCADIA_IN_DEBUG
#   include"core/event/event.hpp"
#endif

auto Arcadia::AppContext::Instance() -> self_type&
{
    static self_type app_context{};
    return app_context;
}

Arcadia::AppContext::AppContext()
{
#ifdef ARCADIA_IN_DEBUG
    auto& set = Arcadia::EventQueue::Instance()
        .debug_excluded_event_type_set;
    set.emplace(typeid(Arcadia::Event::InputCursorPos));
    set.emplace(typeid(Arcadia::Event::InputCursorMove));
    set.emplace(typeid(Arcadia::Event::WindowPos));

#endif // ARCADIA_IN_DEBUG

}
