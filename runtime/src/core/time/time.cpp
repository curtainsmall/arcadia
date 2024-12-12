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

auto Timer::GetDurationSinceStart() -> DurationType
{
    return _Now() - _Start;
}

auto Timer::GetDurationSinceLast() -> DurationType
{
    return _Now() - _Last;
}

auto Timer::Segment() -> DurationType
{
    auto now = _Now();
    auto diff = now - _Last;
    _Last = now;
    return diff;
}

auto Timer::_Now() const->TimePointType
{
    return std::chrono::steady_clock::now();
}