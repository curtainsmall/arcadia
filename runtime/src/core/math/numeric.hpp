#pragma once

#include "platform/api_def.hpp"

namespace Arcadia
{
    template<class Num>
    [[nodiscard]]
    ACDA_API auto IsInRange(Num num, Num min, Num max) -> bool
    {
        return num >= min && num <= max;
    }
}