#pragma once

#include<cassert>
#include<chrono>
#include<cmath>
#include<complex>
#include<cstddef>
#include<cstdlib>
#include<functional>
#include<string>
#include<typeindex>
#include<typeinfo>
#include<variant>

#include"platform/base.hpp"

#define ACDA_ASSERT(x) assert(x)
#define ACDA_UNREACHABLE(msg) ACDA_ASSERT(0 && msg)

#define ACDA_BIND_MEMBER_FN(fn) [this]<class ...Args>(Args&& ...args) -> decltype(auto) { return this->fn(std::forward<Args>(args)...); }
#define ACDA_BIND_MEMBER_FN_ARBITRARY(obj, fn) [&obj]<class ...Args>(Args&& ...args) -> decltype(auto) { return obj.fn(std::forward<Args>(args)...); }
#define ACDA_BIND_MEMBER_FN_ARBITRARY_PTR(obj_ptr, fn) [&obj_ptr]<class ...Args>(Args&& ...args) -> decltype(auto) { return obj_ptr->fn(std::forward<Args>(args)...); }

using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace std::chrono_literals;
using namespace std::complex_literals;

struct Noncopyable
{
protected:
    Noncopyable() = default;
    Noncopyable(const Noncopyable&) = delete;
    auto operator=(const Noncopyable&) = delete;
};

template<class ...Fns>
struct Overloaded: Fns...
{
    using Fns::operator()...;
};

template<class, template<class ...> class>
constexpr bool is_specialization_of = false;
template<template<class...> class T, class ...Args>
constexpr bool is_specialization_of<T<Args...>, T> = true;

template<class Type, template<class ...> class Template>
concept instantiated_from = is_specialization_of<Type, Template>;

template<class ...Args>
struct ParameterPack
{
public:
    using tuple_type = std::tuple<Args...>;

    template<size_t Index>
    using at_t = std::tuple_element_t<Index, tuple_type>;
public:
    static constexpr size_t size_v = std::tuple_size_v<tuple_type>;
};

template<class Enum>
    requires std::is_enum_v<Enum>
ACDA_API auto to_underlying(Enum e) -> std::underlying_type_t<Enum>
{
    return static_cast<std::underlying_type_t<Enum>>(e);
}

template<
    class Ret,
    instantiated_from<std::variant> Variant,
    class ...BranchFns
>
ACDA_API auto match(Variant& variant, BranchFns&& ...fns) -> Ret
{
    return std::visit<Ret>(
        Overloaded{
            std::forward<BranchFns>(fns)...
        },
        variant
    );
}

template<
    class Ret,
    instantiated_from<std::variant> Variant,
    class ...BranchFns
>
ACDA_API auto match(const Variant& variant, BranchFns&& ...fns) -> Ret
{
    return std::visit<Ret>(
        Overloaded{
            std::forward<BranchFns>(fns)...
        },
        variant
    );
}

template<
    class Ret,
    class Case,
    std::convertible_to<Case> Cond,
    class ...Cases
>
    requires (sizeof...(Cases) % 2 == 0)
ACDA_API auto match(const Cond& cond, const Case& case_expr, const std::function<Ret()>& case_fn, Cases&& ...cases) -> Ret
{
    if constexpr(sizeof...(Cases) == 0)
    {
        return cond == case_expr ? case_fn() : Ret();
    }
    else
    {
        return cond == case_expr ? case_fn() : match<Ret, Cond, Case>(cond, std::forward<Cases>(cases)...);
    }
}

template<
    class Ret,
    class Case,
    std::convertible_to<Case> Cond,
    class ...Cases
>
    requires (sizeof...(Cases) % 2 == 0)
ACDA_API auto match(const Cond& cond, const std::function<Ret()>& default_fn, const Case& case_expr, const std::function<Ret()>& case_fn, Cases&& ...cases) -> Ret
{
    if constexpr(sizeof...(Cases) == 0)
    {
        return cond == case_expr ? case_fn() : default_fn();
    }
    else
    {
        return cond == case_expr ? case_fn() : match<Ret, Cond, Case>(cond, std::forward<Cases>(cases)...);
    }
}

template<
    class Res,
    class Case,
    std::convertible_to<Case> Cond,
    class ...Cases
>
    requires (sizeof...(Cases) % 2 == 0)
ACDA_API auto match(const Cond& cond, const Case& case_expr, const Res& case_res, Cases&& ...cases) -> Res
{
    if constexpr(sizeof...(Cases) == 0)
    {
        return cond == case_expr ? case_res : Res();
    }
    else
    {
        return cond == case_expr ? case_res : match<Res, Cond, Case>(cond, std::forward<Cases>(cases)...);
    }
}

template<
    class Res,
    class Case,
    std::convertible_to<Case> Cond,
    class ...Cases
>
    requires (sizeof...(Cases) % 2 == 0)
ACDA_API auto match(const Cond& cond, const Res& default_res, const Case& case_expr, const Res& case_res, Cases&& ...cases) -> Res
{
    if constexpr(sizeof...(Cases) == 0)
    {
        return cond == case_expr ? case_res : default_res;
    }
    else
    {
        return cond == case_expr ? case_res : match<Res, Cond, Case>(cond, std::forward<Cases>(cases)...);
    }
}

template<class Enum>
concept cEnumBitmask = requires{
    std::is_enum_v<Enum>;
    Enum::_EnumBitmap;
};

template<cEnumBitmask Enum>
ACDA_API auto operator|(const Enum& a, const Enum& b) -> Enum
{
    return static_cast<Enum>(to_underlying(a) | to_underlying(b));
}

template<cEnumBitmask Enum>
ACDA_API auto operator|=(Enum& a, const Enum& b) -> Enum&
{
    a = a | b;
    return a;
}

template<cEnumBitmask Enum>
ACDA_API auto operator&(const Enum& a, const Enum& b) -> Enum
{
    return static_cast<Enum>(to_underlying(a) & to_underlying(b));
}

template<cEnumBitmask Enum>
ACDA_API auto operator&=(Enum& a, const Enum& b) -> Enum&
{
    a = a & b;
    return a;
}

template<cEnumBitmask Enum>
ACDA_API auto operator~(const Enum& a) -> Enum
{
    return static_cast<Enum>(~to_underlying(a));
}

template<cEnumBitmask Enum>
ACDA_API auto operator!(const Enum& a) -> bool
{
    return !to_underlying(a);
}

template<class Enum, class Int>
    requires std::is_enum_v<Enum>&& std::is_integral_v<Int>
ACDA_API auto operator==(const Enum& lhs, const Int& rhs) -> bool
{
    return static_cast<Enum>(rhs) == lhs;
}

template<class Enum, class Int>
    requires std::is_enum_v<Enum>&& std::is_integral_v<Int>
ACDA_API auto operator==(const Int& lhs, const Enum& rhs) -> bool
{
    return rhs == lhs;
}

template<cEnumBitmask Enum, class Int>
    requires std::is_integral_v<Int>
ACDA_API auto operator&(const Enum& lhs, const Int& rhs) -> Enum
{
    return static_cast<Enum>(static_cast<Int>(lhs) & rhs);
}

template<cEnumBitmask Enum, class Int>
    requires std::is_integral_v<Int>
ACDA_API auto operator&(const Int& lhs, const Enum& rhs) -> Enum
{
    return rhs & lhs;
}

template<cEnumBitmask Enum, class Int>
    requires std::is_integral_v<Int>
ACDA_API auto operator&=(Enum& lhs, const Int& rhs) -> Enum&
{
    lhs = lhs & rhs;
    return lhs;
}

template<cEnumBitmask Enum, class Int>
    requires std::is_integral_v<Int>
ACDA_API auto operator|(const Enum& lhs, const Int& rhs) -> Enum
{
    return static_cast<Enum>(static_cast<Int>(lhs) | rhs);
}

template<cEnumBitmask Enum, class Int>
    requires std::is_integral_v<Int>
ACDA_API auto operator|(const Int& lhs, const Enum& rhs) -> Enum
{
    return rhs | lhs;
}

template<cEnumBitmask Enum, class Int>
    requires std::is_integral_v<Int>
ACDA_API auto operator|=(Enum& lhs, const Int& rhs) -> Enum&
{
    lhs = lhs | rhs;
    return lhs;
}

template<class T>
ACDA_API auto to_bool(const T& val) -> bool
{
    return static_cast<bool>(val);
}
