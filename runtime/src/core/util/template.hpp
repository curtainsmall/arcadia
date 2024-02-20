#pragma once

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


}
