#pragma once

#include<chrono>

#include"core/base.hpp"

struct Timer
{
public:
    using clock_type = std::chrono::steady_clock;
    using duration_type = clock_type::duration;
    using time_point_type = std::chrono::time_point<clock_type>;
public:
    Timer();

    void Reset();

    auto SinceStart() -> duration_type;
    auto SinceLast() -> duration_type;

private:
    auto _Now() const->time_point_type;
private:
    time_point_type _Start{};
    time_point_type _Last{};
};
