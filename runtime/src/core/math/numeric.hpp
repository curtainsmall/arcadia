#pragma once

#include"core/base.hpp"

namespace Arcadia
{
    template<class Num>
    [[nodiscard]]
    ARCADIA_API auto IsInRange(Num num, Num min, Num max) -> bool
    {
        return num >= min && num <= max;
    }
}
