#pragma once

#include <tuple>
#include <variant>

namespace Arcadia
{
    template<class, template<class ...> class>
    constexpr bool IsSpecializationOf = false;

    template<template<class...> class T, class ...Args>
    constexpr bool IsSpecializationOf<T<Args...>, T> = true;

    namespace Concepts
    {
        template<class Type, template<class ...> class Template>
        concept InstantiatedFrom = IsSpecializationOf<Type, Template>;
    }

    template<class, class ...>
    constexpr bool IsTypeInTuple = false;

    template<class T, class ...Args>
    constexpr bool IsTypeInTuple<T, std::tuple<Args...>> = (std::is_same_v<T, Args> || ...);

    namespace Concepts
    {
        template<class T, class Tuple>
        concept TupleContainsType = IsTypeInTuple<T, Tuple>;
    }

    template<class, class ...>
    constexpr bool IsTypeInVariant = false;

    template<class T, class ...Args>
    constexpr bool IsTypeInVariant<T, std::variant<Args...>> = (std::is_same_v<T, Args> || ...);

    namespace Concepts
    {
        template<class T, class Variant>
        concept VariantContainsType = IsTypeInVariant<T, Variant>;
    };
}