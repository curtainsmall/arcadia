#pragma once

#include<concepts>

#include"core/match.hpp"

namespace Arcadia
{
    template<typename ...Args>
    class ParameterPack
    {
    public:
        using TupleType = std::tuple<Args...>;
        using SelfType = ParameterPack<Args...>;

        template<std::size_t Index>
        using ElementTypeAt = std::tuple_element_t<Index, TupleType>;

    public:
        template<typename T>
        static constexpr bool ContainsType = IsTypeInTuple<T, TupleType>;

        static constexpr std::size_t Size = std::tuple_size_v<TupleType>;
    };

    namespace Concepts
    {
        template<
            typename T,
            typename Pack
        >
        concept TypeInParameterPack =
            Concepts::InstantiatedFrom<Pack, ParameterPack>
            && Pack::template ContainsType<T>;
    }
}