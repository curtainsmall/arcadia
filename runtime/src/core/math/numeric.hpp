#pragma once

#include"core/base.hpp"

template<class Num>
[[nodiscard]]
ACDA_API auto is_in_range(Num num, Num min, Num max) -> bool
{
    return num >= min && num <= max;
}
