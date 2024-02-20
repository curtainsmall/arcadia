#pragma once

#include<concepts>
#include<functional>

namespace arcadia
{
    template<class F>
    auto make_funtion(F ptr)
    {
        return std::function<std::remove_pointer_t<F>>{ ptr };
    }
}

