#pragma once

#include <type_traits>

#include "platform/defines.hpp"

namespace Arcadia
{
    template<class Enum>
        requires std::is_enum_v<Enum>
    [[nodiscard]]
    constexpr auto ToUnderlying(Enum e) -> std::underlying_type_t<Enum>
    {
        return static_cast<std::underlying_type_t<Enum>>(e);
    }

    namespace Concepts
    {
        template<class T>
        concept EnumBitfield =
            std::is_enum_v<T>
            && requires{ T::_EnumBitfield; };
    }

    template<Concepts::EnumBitfield Enum>
    [[nodiscard]]
    auto operator|(const Enum& a, const Enum& b) -> Enum
    {
        return static_cast<Enum>(ToUnderlying(a) | ToUnderlying(b));
    }

    template<Concepts::EnumBitfield Enum>
    auto operator|=(Enum& a, const Enum& b) -> Enum&
    {
        a = a | b;
        return a;
    }

    template<Concepts::EnumBitfield Enum>
    [[nodiscard]]
    auto operator&(const Enum& a, const Enum& b) -> Enum
    {
        return static_cast<Enum>(ToUnderlying(a) & ToUnderlying(b));
    }

    template<Concepts::EnumBitfield Enum>
    auto operator&=(Enum& a, const Enum& b) -> Enum&
    {
        a = a & b;
        return a;
    }

    template<Concepts::EnumBitfield Enum>
    [[nodiscard]]
    auto operator~(const Enum& a) -> Enum
    {
        return static_cast<Enum>(~ToUnderlying(a));
    }

    template<Concepts::EnumBitfield Enum>
    [[nodiscard]]
    auto operator!(const Enum& a) -> bool
    {
        return !ToUnderlying(a);
    }

    template<class Enum, class Int>
        requires std::is_enum_v<Enum>&& std::is_integral_v<Int>
    [[nodiscard]]
    auto operator==(const Enum& lhs, const Int& rhs) -> bool
    {
        return static_cast<Enum>(rhs) == lhs;
    }

    template<class Enum, class Int>
        requires std::is_enum_v<Enum>&& std::is_integral_v<Int>
    [[nodiscard]]
    auto operator==(const Int& lhs, const Enum& rhs) -> bool
    {
        return rhs == lhs;
    }

    template<Concepts::EnumBitfield Enum, class Int>
        requires std::is_integral_v<Int>
    [[nodiscard]]
    auto operator&(const Enum& lhs, const Int& rhs) -> Enum
    {
        return static_cast<Enum>(static_cast<Int>(lhs) & rhs);
    }

    template<Concepts::EnumBitfield Enum, class Int>
        requires std::is_integral_v<Int>
    [[nodiscard]]
    auto operator&(const Int& lhs, const Enum& rhs) -> Enum
    {
        return rhs & lhs;
    }

    template<Concepts::EnumBitfield Enum, class Int>
        requires std::is_integral_v<Int>
    auto operator&=(Enum& lhs, const Int& rhs) -> Enum&
    {
        lhs = lhs & rhs;
        return lhs;
    }

    template<Concepts::EnumBitfield Enum, class Int>
        requires std::is_integral_v<Int>
    [[nodiscard]]
    auto operator|(const Enum& lhs, const Int& rhs) -> Enum
    {
        return static_cast<Enum>(static_cast<Int>(lhs) | rhs);
    }

    template<Concepts::EnumBitfield Enum, class Int>
        requires std::is_integral_v<Int>
    [[nodiscard]]
    auto operator|(const Int& lhs, const Enum& rhs) -> Enum
    {
        return rhs | lhs;
    }

    template<Concepts::EnumBitfield Enum, class Int>
        requires std::is_integral_v<Int>
    auto operator|=(Enum& lhs, const Int& rhs) -> Enum&
    {
        lhs = lhs | rhs;
        return lhs;
    }
}