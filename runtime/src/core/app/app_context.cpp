#include "pch.hpp"
#include "app_context.hpp"

auto arcadia::app_context::instance() -> self_type&
{
    static self_type app_context{};
    return app_context;
}
