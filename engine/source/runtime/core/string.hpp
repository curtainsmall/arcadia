#pragma once

#include <string>

#include "platform/defines.hpp"

namespace Arcadia
{
    template<std::size_t N>
    struct ACDA_API StringLiteral
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