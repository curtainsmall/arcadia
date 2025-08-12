#pragma once

#include <utility>

#include "core/uuid.hpp"

namespace Arcadia
{
    template<class Value>
    struct ACDA_API Identifiable
    {
    public:
        using ValueType = Value;
        using SelfType = Identifiable<ValueType>;
    public:
        Identifiable() = default;
        Identifiable(const ValueType& val):
            _Value(val)
        {
        }
        Identifiable(ValueType&& val):
            _Value(val)
        {
        }
        template<class ...Args>
        Identifiable(Args&& ...args) :
            _Value(std::forward<Args>(args)...)
        {
        }

        Identifiable(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;

        Identifiable(SelfType&&) noexcept = default;
        auto operator=(SelfType&&) noexcept -> SelfType & = default;

        template<std::size_t Index>
        [[nodiscard]]
        auto get() const -> const auto&
        {
            static_assert(Index == 0 || Index == 1, "Invalid index");

            if constexpr(Index == 0)
            {
                return GetUuid();
            }
            else
            {
                return GetValue();
            }
        }

        [[nodiscard]]
        auto GetUuid() const -> const Uuid&
        {
            return _Uuid;
        }

        [[nodiscard]]
        auto GetValue() const -> const ValueType&
        {
            return _Value;
        }

    private:
        Uuid _Uuid{};
        ValueType _Value{};
    };
}

namespace std
{
    template<class Value>
    struct ACDA_API tuple_size<Arcadia::Identifiable<Value>>:
        public std::integral_constant<std::size_t, 2>
    {
    };

    template<std::size_t Index, class Value>
    struct ACDA_API tuple_element<Index, Arcadia::Identifiable<Value>>:
        public std::tuple_element<Index, std::tuple<Arcadia::Uuid, Value>>
    {
    };
}