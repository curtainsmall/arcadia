#pragma once

#include"boost/functional/hash.hpp"

#include"core/base.hpp"

namespace Arcadia
{
    template<class Type>
    ARCADIA_API auto HashCombine(const std::size_t& seed, const Type& val) -> std::size_t
    {
        std::size_t res{ seed };
        boost::hash_combine(res, val);
        return res;
    }

    template<class Type, class ...Types>
    ARCADIA_API auto Hash(const Type& val, const Types& ...vals) -> std::size_t
    {
        if constexpr(sizeof...(Types) == 0)
        {
            return std::hash<Type>{}(val);
        }
        else
        {
            return Arcadia::HashCombine(
                Arcadia::Hash(vals...),
                val
            );
        }
    }
}
