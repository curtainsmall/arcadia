#pragma once

#include"boost/functional/hash.hpp"

#include"core/base.hpp"

template<typename Type>
ACDA_API auto CombineHash(const size_t& seed, const Type& val) -> size_t
{
    size_t res{ seed };
    boost::hash_combine(res, val);
    return res;
}

template<typename Type, typename ...Types>
ACDA_API auto Hash(const Type& val, const Types& ...vals) -> size_t
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