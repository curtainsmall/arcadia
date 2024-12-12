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

#define ACDA_BIND_MEMBER_FN(fn) [this]<typename ...Args>(Args&& ...args) -> decltype(auto) { return this->fn(std::forward<Args>(args)...); }
#define ACDA_BIND_MEMBER_FN_ARBITRARY(obj, fn) [&obj]<typename ...Args>(Args&& ...args) -> decltype(auto) { return obj.fn(std::forward<Args>(args)...); }
#define ACDA_BIND_MEMBER_FN_ARBITRARY_PTR(obj_ptr, fn) [&obj_ptr]<typename ...Args>(Args&& ...args) -> decltype(auto) { return obj_ptr->fn(std::forward<Args>(args)...); }

using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace std::chrono_literals;
using namespace std::complex_literals;

namespace Arcadia
{
    using UnicodeCodepoint = uint32_t;

    class Noncopyable
    {
    protected:
        Noncopyable() = default;
        Noncopyable(const Noncopyable&) = delete;
        auto operator=(const Noncopyable&) = delete;
    };

    template<typename ...Fns>
    class OverloadedFunctionsWrapper: public Fns...
    {
    public:
        using Fns::operator()...;
    };

    template<typename, template<typename ...> typename>
    constexpr bool IsSpecializationOf = false;
    template<template<typename...> typename T, typename ...Args>
    constexpr bool IsSpecializationOf<T<Args...>, T> = true;

    template<typename Type, template<typename ...> typename Template>
    concept cInstantiatedFrom = IsSpecializationOf<Type, Template>;

    template<typename ...Args>
    class ParameterPack
    {
    public:
        using TupleType = std::tuple<Args...>;

        template<size_t Index>
        using ElementTypeAt = std::tuple_element_t<Index, TupleType>;
    public:
        static constexpr size_t Size = std::tuple_size_v<TupleType>;
    };

    template<typename Enum>
        requires std::is_enum_v<Enum>
    ACDA_API auto ToUnderlying(Enum e) -> std::underlying_type_t<Enum>
    {
        return static_cast<std::underlying_type_t<Enum>>(e);
    }

    template<
        typename Ret,
        cInstantiatedFrom<std::variant> Variant,
        typename ...BranchFns
    >
    ACDA_API auto Match(Variant& variant, BranchFns&& ...fns) -> Ret
    {
        return std::visit<Ret>(
            OverloadedFunctionsWrapper{
                std::forward<BranchFns>(fns)...
            },
            variant
        );
    }

    template<
        typename Ret,
        cInstantiatedFrom<std::variant> Variant,
        typename ...BranchFns
    >
    ACDA_API auto Match(const Variant& variant, BranchFns&& ...fns) -> Ret
    {
        return std::visit<Ret>(
            OverloadedFunctionsWrapper{
                std::forward<BranchFns>(fns)...
            },
            variant
        );
    }

    template<
        typename Ret,
        typename Case,
        std::convertible_to<Case> Cond,
        typename ...Cases
    >
        requires (sizeof...(Cases) % 2 == 0)
    ACDA_API auto Match(const Cond& cond, const Case& case_expr, const std::function<Ret()>& case_fn, Cases&& ...cases) -> Ret
    {
        if constexpr(sizeof...(Cases) == 0)
        {
            return cond == case_expr ? case_fn() : Ret();
        }
        else
        {
            return cond == case_expr ? case_fn() : Match<Ret, Cond, Case>(cond, std::forward<Cases>(cases)...);
        }
    }

    template<
        typename Ret,
        typename Case,
        std::convertible_to<Case> Cond,
        typename ...Cases
    >
        requires (sizeof...(Cases) % 2 == 0)
    ACDA_API auto Match(const Cond& cond, const std::function<Ret()>& default_fn, const Case& case_expr, const std::function<Ret()>& case_fn, Cases&& ...cases) -> Ret
    {
        if constexpr(sizeof...(Cases) == 0)
        {
            return cond == case_expr ? case_fn() : default_fn();
        }
        else
        {
            return cond == case_expr ? case_fn() : Match<Ret, Cond, Case>(cond, std::forward<Cases>(cases)...);
        }
    }

    template<
        typename Res,
        typename Case,
        std::convertible_to<Case> Cond,
        typename ...Cases
    >
        requires (sizeof...(Cases) % 2 == 0)
    ACDA_API auto Match(const Cond& cond, const Case& case_expr, const Res& case_res, Cases&& ...cases) -> Res
    {
        if constexpr(sizeof...(Cases) == 0)
        {
            return cond == case_expr ? case_res : Res();
        }
        else
        {
            return cond == case_expr ? case_res : Match<Res, Cond, Case>(cond, std::forward<Cases>(cases)...);
        }
    }

    template<
        typename Res,
        typename Case,
        std::convertible_to<Case> Cond,
        typename ...Cases
    >
        requires (sizeof...(Cases) % 2 == 0)
    ACDA_API auto Match(const Cond& cond, const Res& default_res, const Case& case_expr, const Res& case_res, Cases&& ...cases) -> Res
    {
        if constexpr(sizeof...(Cases) == 0)
        {
            return cond == case_expr ? case_res : default_res;
        }
        else
        {
            return cond == case_expr ? case_res : Match<Res, Cond, Case>(cond, std::forward<Cases>(cases)...);
        }
    }

    template<typename Enum>
    concept cEnumBitmask = requires{
        std::is_enum_v<Enum>;
        Enum::_EnumBitmap;
    };

    template<cEnumBitmask Enum>
    ACDA_API auto operator|(const Enum& a, const Enum& b) -> Enum
    {
        return static_cast<Enum>(ToUnderlying(a) | ToUnderlying(b));
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
        return static_cast<Enum>(ToUnderlying(a) & ToUnderlying(b));
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
        return static_cast<Enum>(~ToUnderlying(a));
    }

    template<cEnumBitmask Enum>
    ACDA_API auto operator!(const Enum& a) -> bool
    {
        return !ToUnderlying(a);
    }

    template<typename Enum, typename Int>
        requires std::is_enum_v<Enum>&& std::is_integral_v<Int>
    ACDA_API auto operator==(const Enum& lhs, const Int& rhs) -> bool
    {
        return static_cast<Enum>(rhs) == lhs;
    }

    template<typename Enum, typename Int>
        requires std::is_enum_v<Enum>&& std::is_integral_v<Int>
    ACDA_API auto operator==(const Int& lhs, const Enum& rhs) -> bool
    {
        return rhs == lhs;
    }

    template<cEnumBitmask Enum, typename Int>
        requires std::is_integral_v<Int>
    ACDA_API auto operator&(const Enum& lhs, const Int& rhs) -> Enum
    {
        return static_cast<Enum>(static_cast<Int>(lhs) & rhs);
    }

    template<cEnumBitmask Enum, typename Int>
        requires std::is_integral_v<Int>
    ACDA_API auto operator&(const Int& lhs, const Enum& rhs) -> Enum
    {
        return rhs & lhs;
    }

    template<cEnumBitmask Enum, typename Int>
        requires std::is_integral_v<Int>
    ACDA_API auto operator&=(Enum& lhs, const Int& rhs) -> Enum&
    {
        lhs = lhs & rhs;
        return lhs;
    }

    template<cEnumBitmask Enum, typename Int>
        requires std::is_integral_v<Int>
    ACDA_API auto operator|(const Enum& lhs, const Int& rhs) -> Enum
    {
        return static_cast<Enum>(static_cast<Int>(lhs) | rhs);
    }

    template<cEnumBitmask Enum, typename Int>
        requires std::is_integral_v<Int>
    ACDA_API auto operator|(const Int& lhs, const Enum& rhs) -> Enum
    {
        return rhs | lhs;
    }

    template<cEnumBitmask Enum, typename Int>
        requires std::is_integral_v<Int>
    ACDA_API auto operator|=(Enum& lhs, const Int& rhs) -> Enum&
    {
        lhs = lhs | rhs;
        return lhs;
    }

    template<typename T>
    ACDA_API auto ToBool(const T& val) -> bool
    {
        return static_cast<bool>(val);
    }
}