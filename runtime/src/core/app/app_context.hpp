#pragma once

#include"core/base.hpp"
#include"core/time/time.hpp"
#ifdef ACDA_DEBUG_MODE
#   include"function/input/input_events.hpp"
#endif

class AppContext
{
public:
    using SelfType = AppContext;
public:
    static auto Instance() -> SelfType&;

    AppContext();

public:
    bool Running{ false };
    Timer Timer{};
    std::chrono::nanoseconds DeltaTime{};
};