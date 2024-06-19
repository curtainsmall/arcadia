#include"pch.hpp"
#include"time.hpp"

Timer::Timer()
{
    reset();
}

void Timer::reset()
{
    _Start = _now();
    _Last = _Start;
}

auto Timer::since_start() -> duration_type
{
    return _now() - _Start;
}

auto Timer::since_last() -> duration_type
{
    auto now = _now();
    auto diff = now - _Last;
    _Last = now;
    return diff;
}

auto Timer::_now() const->time_point_type
{
    return std::chrono::steady_clock::now();
}