#pragma once

#include "core/time.hpp"
#include "platform/defines.hpp"


#ifdef ACDA_DEBUG_MODE
#   include"function/input/input_events.hpp"
#endif

namespace Arcadia
{
    struct ACDA_API AppContext
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