#pragma once

#include<utility>

#include"core/noncopyable.hpp"
#include"core/uuid.hpp"

namespace Arcadia
{
    template<typename Value>
    class Identifiable: public Noncopyable
    {
    public:
        using ValueType = Value;
        using SelfType = Identifiable<ValueType>;
    public:
        Identifiable() = default;
        Identifiable(const ValueType& val) :
            _Value(val)
        {}
        Identifiable(ValueType&& val) :
            _Value(val)
        {}
        template<typename ...Args>
        Identifiable(Args&& ...args) :
            _Value(std::forward<Args>(args)...)
        {}

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
    template<typename Value>
    class tuple_size<Arcadia::Identifiable<Value>>:
        public std::integral_constant<std::size_t, 2>
    {};

    template<std::size_t Index, typename Value>
    class tuple_element<Index, Arcadia::Identifiable<Value>>:
        public std::tuple_element<Index, std::tuple<Arcadia::Uuid, Value>>
    {};
}