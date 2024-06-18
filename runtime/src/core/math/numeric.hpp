#pragma once

#include"core/base.hpp"

template<class Num>
[[nodiscard]]
ACDA_API auto IsInRange(Num num, Num min, Num max) -> bool
{
    return num >= min && num <= max;
}
