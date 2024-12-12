#pragma once

#include<string>

#include"core/base.hpp"

namespace Arcadia
{
    template<size_t N>
    class StringLiteral
    {
    public:
        constexpr StringLiteral(const char(&str)[N])
        {
            std::copy_n(str, N, Value);
        }

        operator std::string() const
        {
            return std::string{ Value[0], Value[N - 1] };
        }
    public:
        char Value[N];
    };
}