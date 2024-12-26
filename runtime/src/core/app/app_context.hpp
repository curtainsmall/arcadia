#pragma once

#include"core/time/time.hpp"
#include"platform/api_def.hpp"
#include"platform/debug_def.hpp"

#ifdef ACDA_DEBUG_MODE
#   include"function/input/input_events.hpp"
#endif

namespace Arcadia
{
    class AppContext
    {
    public:
        using SelfType = AppContext;
    public:
        static auto Instance() -> SelfType&;

    public:
        bool Running{ false };
        Timer Timer{};
        std::chrono::nanoseconds DeltaTime{};
    };
}