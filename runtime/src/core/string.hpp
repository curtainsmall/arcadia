#pragma once

#include <string>

#include "platform/api_def.hpp"

namespace Arcadia
{
    template<std::size_t N>
    struct StringLiteral
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