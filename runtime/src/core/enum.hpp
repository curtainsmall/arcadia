#pragma once

#include<type_traits>

#include"platform/api_def.hpp"

namespace Arcadia
{
    template<typename Enum>
        requires std::is_enum_v<Enum>
    ACDA_API auto ToUnderlying(Enum e) -> std::underlying_type_t<Enum>
    {
        return static_cast<std::underlying_type_t<Enum>>(e);
    }

    template<typename Enum>
    concept cEnumBitfield =
        std::is_enum_v<Enum>
        && requires{ Enum::_EnumBitfield; };

    template<cEnumBitfield Enum>
    ACDA_API auto operator|(const Enum& a, const Enum& b) -> Enum
    {
        return static_cast<Enum>(ToUnderlying(a) | ToUnderlying(b));
    }

    template<cEnumBitfield Enum>
    ACDA_API auto operator|=(Enum& a, const Enum& b) -> Enum&
    {
        a = a | b;
        return a;
    }

    template<cEnumBitfield Enum>
    ACDA_API auto operator&(const Enum& a, const Enum& b) -> Enum
    {
        return static_cast<Enum>(ToUnderlying(a) & ToUnderlying(b));
    }

    template<cEnumBitfield Enum>
    ACDA_API auto operator&=(Enum& a, const Enum& b) -> Enum&
    {
        a = a & b;
        return a;
    }

    template<cEnumBitfield Enum>
    ACDA_API auto operator~(const Enum& a) -> Enum
    {
        return static_cast<Enum>(~ToUnderlying(a));
    }

    template<cEnumBitfield Enum>
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

    template<cEnumBitfield Enum, typename Int>
        requires std::is_integral_v<Int>
    ACDA_API auto operator&(const Enum& lhs, const Int& rhs) -> Enum
    {
        return static_cast<Enum>(static_cast<Int>(lhs) & rhs);
    }

    template<cEnumBitfield Enum, typename Int>
        requires std::is_integral_v<Int>
    ACDA_API auto operator&(const Int& lhs, const Enum& rhs) -> Enum
    {
        return rhs & lhs;
    }

    template<cEnumBitfield Enum, typename Int>
        requires std::is_integral_v<Int>
    ACDA_API auto operator&=(Enum& lhs, const Int& rhs) -> Enum&
    {
        lhs = lhs & rhs;
        return lhs;
    }

    template<cEnumBitfield Enum, typename Int>
        requires std::is_integral_v<Int>
    ACDA_API auto operator|(const Enum& lhs, const Int& rhs) -> Enum
    {
        return static_cast<Enum>(static_cast<Int>(lhs) | rhs);
    }

    template<cEnumBitfield Enum, typename Int>
        requires std::is_integral_v<Int>
    ACDA_API auto operator|(const Int& lhs, const Enum& rhs) -> Enum
    {
        return rhs | lhs;
    }

    template<cEnumBitfield Enum, typename Int>
        requires std::is_integral_v<Int>
    ACDA_API auto operator|=(Enum& lhs, const Int& rhs) -> Enum&
    {
        lhs = lhs | rhs;
        return lhs;
    }
}