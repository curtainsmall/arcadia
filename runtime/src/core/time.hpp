#pragma once

#include <chrono>

#include "platform/api_def.hpp"

namespace Arcadia
{
    class Timer
    {
    public:
        using ClockType = std::chrono::steady_clock;
        using DurationType = ClockType::duration;
        using TimePointType = std::chrono::time_point<ClockType>;
    public:
        Timer();

        void Reset();

        auto GetDurationSinceStart() -> DurationType;
        auto GetDurationSinceLast() -> DurationType;
        auto Segment() -> DurationType;

    private:
        auto _Now() const->TimePointType;
    private:
        TimePointType _Start{};
        TimePointType _Last{};
    };
}