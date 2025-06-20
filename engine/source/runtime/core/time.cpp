#include "pch.hpp"
#include "time.hpp"

Arcadia::Timer::Timer()
{
    Reset();
}

void Arcadia::Timer::Reset()
{
    _Start = _Now();
    _Last = _Start;
}

auto Arcadia::Timer::GetDurationSinceStart() -> DurationType
{
    return _Now() - _Start;
}

auto Arcadia::Timer::GetDurationSinceLast() -> DurationType
{
    return _Now() - _Last;
}

auto Arcadia::Timer::Segment() -> DurationType
{
    auto now = _Now();
    auto diff = now - _Last;
    _Last = now;
    return diff;
}

auto Arcadia::Timer::_Now() const->TimePointType
{
    return std::chrono::steady_clock::now();
}