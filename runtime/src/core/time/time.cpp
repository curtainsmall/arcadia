#include"pch.hpp"
#include"time.hpp"

Timer::Timer()
{
    Reset();
}

void Timer::Reset()
{
    _Start = _Now();
    _Last = _Start;
}

auto Timer::SinceStart() -> duration_type
{
    return _Now() - _Start;
}

auto Timer::SinceLast() -> duration_type
{
    auto now = _Now();
    auto diff = now - _Last;
    _Last = now;
    return diff;
}

auto Timer::_Now() const->time_point_type
{
    return std::chrono::steady_clock::now();
}