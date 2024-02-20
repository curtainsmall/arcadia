#pragma once

#include"core/base.hpp"

namespace arcadia
{
    template<std::size_t N>
    struct ARCADIA_API string_literal
    {
    public:
        constexpr string_literal(const char(&str)[N])
        {
            std::copy_n(str, N, value);
        }
    public:
        char value[N];
    };
}
