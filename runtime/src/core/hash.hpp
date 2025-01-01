#pragma once

#include"boost/functional/hash.hpp"

#include"platform/api_def.hpp"

namespace Arcadia
{
    template<typename Type>
    [[nodiscard]]
    ACDA_API auto CombineHash(const std::size_t& seed, const Type& val) -> std::size_t
    {
        std::size_t res{ seed };
        boost::hash_combine(res, val);
        return res;
    }

    template<typename Type, typename ...Types>
    [[nodiscard]]
    ACDA_API auto Hash(const Type& val, const Types& ...vals) -> std::size_t
    {
        if constexpr(sizeof...(Types) == 0)
        {
            return std::hash<Type>{}(val);
        }
        else
        {
            return CombineHash(
                Hash(vals...),
                val
            );
        }
    }
}