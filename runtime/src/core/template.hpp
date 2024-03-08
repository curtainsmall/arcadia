#pragma once

#include<tuple>

#include"core/base.hpp"

namespace arcadia
{
    template<class ...Fns>
    struct ARCADIA_API overloaded: Fns...
    {
        using Fns::operator()...;
    };

    template<class, template<class ...> class>
    inline constexpr bool is_specialization_of = false;
    template<template<class...> class T, class ...Args>
    inline constexpr bool is_specialization_of<T<Args...>, T> = true;

    template<class Type, template<class ...> class Template>
    concept instantiated_from = arcadia::is_specialization_of<Type, Template>;

    /*template<class Type, template<class ...> class Template>
    concept instantiated_from = requires(Type t)
    {
        [] <class ...Types>(const Template<Types...>&)
        {}(t);
    };*/

    template<class ...Args>
    struct ARCADIA_API pack
    {
    public:
        using tuple_type = std::tuple<Args...>;

        template<std::size_t Index>
        using at_t = std::tuple_element_t<Index, tuple_type>;
    public:
        static constexpr std::size_t size = std::tuple_size_v<tuple_type>;
    };


}
