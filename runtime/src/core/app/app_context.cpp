#include "pch.hpp"

#include "app_context.hpp"

#include "platform/debug_def.hpp"

#ifdef ACDA_DEBUG_MODE
#   include"core/event.hpp"
#endif

auto Arcadia::AppContext::Instance() -> SelfType&
{
    static SelfType app_context{};
    return app_context;
}