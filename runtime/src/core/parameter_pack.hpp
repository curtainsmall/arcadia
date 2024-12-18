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

    template<
        typename T,
        typename Pack
    >
    concept cTypeInParameterPack = requires{
        cInstantiatedFrom<Pack, ParameterPack>;
        cTupleContainsType<T, typename Pack::TupleType>;
    };
}