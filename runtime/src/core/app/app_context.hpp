#pragma once

#include"core/base.hpp"
#include"core/time/timer.hpp"
#ifdef ARCADIA_IN_DEBUG
#   include"function/input/input_events.hpp"
#endif

struct AppContext
{
public:
    using self_type = AppContext;
public:
    static auto Instance() -> self_type&;

    AppContext();

public:
    bool Running{ false };
    Timer Timer{};
    Timer::duration_type DeltaTime{};
};
