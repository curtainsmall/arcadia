#pragma once

#include"core/base.hpp"

namespace arcadia
{
    template<class Num>
    [[nodiscard]]
    ARCADIA_API auto is_in_range(Num num, Num min, Num max) -> bool
    {
        return num >= min && num <= max;
    }
}
