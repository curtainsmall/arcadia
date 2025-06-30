#pragma once

#include "platform/defines.hpp"

namespace Arcadia
{
    template<class Num>
    [[nodiscard]]
    auto IsInRange(Num num, Num min, Num max) -> bool
    {
        return num >= min && num <= max;
    }
}