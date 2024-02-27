#pragma once

#include"core/base.hpp"
#include"core/time/timer.hpp"

namespace arcadia
{
    struct ARCADIA_API app_context
    {
    public:
        using self_type = app_context;
    public:
        static auto instance() -> self_type&;
    public:
        bool running{ false };
        arcadia::timer timer{};
    };
}
