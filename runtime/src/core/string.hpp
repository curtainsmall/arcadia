#pragma once

#include<string>

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

        operator std::string() const
        {
            return std::string{ value[0], value[N - 1] };
        }
    public:
        char value[N];
    };
}
