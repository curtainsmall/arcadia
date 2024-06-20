#pragma once

#include"core/base.hpp"
#include"core/time/time.hpp"
#ifdef ACDA_DEBUG_MODE
#   include"function/input/input_events.hpp"
#endif

struct AppContext
{
public:
    using self_type = AppContext;
public:
    static auto instance() -> self_type&;

    AppContext();

public:
    bool running{ false };
    Timer timer{};
    std::chrono::nanoseconds delta_time{};
};
