#pragma once

#include"core/base.hpp"
#include"core/time/timer.hpp"
#ifdef ARCADIA_IN_DEBUG
#   include"function/input/input_events.hpp"
#endif

namespace arcadia
{
    struct ARCADIA_API app_context
    {
    public:
        using self_type = app_context;
    public:
        static auto instance() -> self_type&;

        app_context();

    public:
        bool running{ false };
        arcadia::timer timer{};
        arcadia::timer::duration_type delta_time{};
    };
}
