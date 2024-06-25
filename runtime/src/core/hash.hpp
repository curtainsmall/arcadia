#pragma once

#include"boost/functional/hash.hpp"

#include"core/base.hpp"

template<class Type>
ACDA_API auto hash_combine(const size_t& seed, const Type& val) -> size_t
{
    size_t res{ seed };
    boost::hash_combine(res, val);
    return res;
}

template<class Type, class ...Types>
ACDA_API auto hash(const Type& val, const Types& ...vals) -> size_t
{
    if constexpr(sizeof...(Types) == 0)
    {
        return std::hash<Type>{}(val);
    }
    else
    {
        return hash_combine(
            hash(vals...),
            val
        );
    }
}
