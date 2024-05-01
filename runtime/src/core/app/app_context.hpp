#pragma once

#include"core/base.hpp"
#include"core/time/timer.hpp"
#ifdef ARCADIA_IN_DEBUG
#   include"function/input/input_events.hpp"
#endif

namespace Arcadia
{
    struct AppContext
    {
    public:
        using self_type = AppContext;
    public:
        static auto Instance() -> self_type&;

        AppContext();

    public:
        bool Running{ false };
        Arcadia::Timer Timer{};
        Arcadia::Timer::duration_type DeltaTime{};
    };
}
