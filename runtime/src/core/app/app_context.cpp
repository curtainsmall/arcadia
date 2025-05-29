#include "pch.hpp"

#include "app_context.hpp"

auto Arcadia::AppContext::Instance() -> SelfType&
{
    static SelfType app_context {};
    return app_context;
}