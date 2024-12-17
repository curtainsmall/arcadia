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

        template<std::size_t Index>
        using ElementTypeAt = std::tuple_element_t<Index, TupleType>;

    public:
        static constexpr std::size_t Size = std::tuple_size_v<TupleType>;
    };

    template<typename, typename ...>
    constexpr bool IsTypeInTuple = false;

    template<typename T, typename ...Args>
    constexpr bool IsTypeInTuple<T, std::tuple<Args...>> = (std::is_same_v<T, Args> || ...);

    template<
        typename T,
        typename Pack
    >
    concept cTypeInParameterPack = requires{
        cInstantiatedFrom<Pack, ParameterPack>;
        IsTypeInTuple<T, typename Pack::TupleType>;
    };
}