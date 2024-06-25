#pragma once

#include<string>

#include"core/base.hpp"

template<size_t N>
struct StringLiteral
{
public:
    constexpr StringLiteral(const char(&str)[N])
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
