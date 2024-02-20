#pragma once

#include<chrono>

#include"core/base.hpp"

namespace arcadia
{
    struct ARCADIA_API timer
    {
    public:
        using clock_type = std::chrono::steady_clock;
        using duration_type = std::chrono::milliseconds;
        using time_point_type = std::chrono::time_point<clock_type, duration_type>;
    public:
        inline timer()
        {
            reset();
        }

        inline void reset()
        {
            _start = _now();
            _last = _start;
        }

        template<class ToDuration = duration_type>
        auto since_start() -> duration_type
        {
            return std::chrono::duration_cast<ToDuration>(_now() - _start);
        }

        template<class ToDuration = duration_type>
        auto since_last() -> duration_type
        {
            auto now = _now();
            auto diff = now - _last;
            _last = now;
            return std::chrono::duration_cast<ToDuration>(diff);
        }

    private:
        inline auto _now() const->time_point_type
        {
            return std::chrono::time_point_cast<duration_type>(std::chrono::steady_clock::now());
        }
    private:
        time_point_type _start{};
        time_point_type _last{};
    };
}
