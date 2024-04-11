#pragma once

#include<chrono>

#include"core/base.hpp"

namespace Arcadia
{
    struct ARCADIA_API Timer
    {
    public:
        using clock_type = std::chrono::steady_clock;
        using duration_type = std::chrono::milliseconds;
        using time_point_type = std::chrono::time_point<clock_type, duration_type>;
    public:
        Timer()
        {
            Reset();
        }

        void Reset()
        {
            _Start = _Now();
            _Last = _Start;
        }

        template<class ToDuration = duration_type>
        auto SinceStart() -> duration_type
        {
            return std::chrono::duration_cast<ToDuration>(_Now() - _Start);
        }

        template<class ToDuration = duration_type>
        auto SinceLast() -> duration_type
        {
            auto now = _Now();
            auto diff = now - _Last;
            _Last = now;
            return std::chrono::duration_cast<ToDuration>(diff);
        }

    private:
        auto _Now() const->time_point_type
        {
            return std::chrono::time_point_cast<duration_type>(std::chrono::steady_clock::now());
        }
    private:
        time_point_type _Start{};
        time_point_type _Last{};
    };
}
