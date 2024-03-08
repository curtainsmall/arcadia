#pragma once

#include"boost/functional/hash.hpp"

#include"core/base.hpp"

namespace arcadia
{
    template<class Type>
    ARCADIA_API auto hash_combine(const std::size_t& val, const Type& type) -> std::size_t
    {
        std::size_t res{ val };
        boost::hash_combine(res, type);
        return res;
    }

    template<class Type, class ...Types>
    ARCADIA_API auto hash(const Type& type, const Types& ...types) -> std::size_t
    {
        if constexpr(sizeof...(Types) == 0)
        {
            return std::hash<Type>{}(type);
        }
        else
        {
            return arcadia::hash_combine(
                arcadia::hash(types...),
                type
            );
        }
    }
}
