#pragma once

#include<tuple>

namespace Arcadia
{
    template<typename, template<typename ...> typename>
    constexpr bool IsSpecializationOf = false;

    template<template<typename...> typename T, typename ...Args>
    constexpr bool IsSpecializationOf<T<Args...>, T> = true;

    template<typename Type, template<typename ...> typename Template>
    concept cInstantiatedFrom = IsSpecializationOf<Type, Template>;

    template<typename, typename ...>
    constexpr bool IsTypeInTuple = false;

    template<typename T, typename ...Args>
    constexpr bool IsTypeInTuple<T, std::tuple<Args...>> = (std::is_same_v<T, Args> || ...);

    template<typename T, typename Tuple>
    concept cTupleContainsType = requires{
        cInstantiatedFrom<Tuple, std::tuple>;
        IsTypeInTuple<T, Tuple>;
    };
}