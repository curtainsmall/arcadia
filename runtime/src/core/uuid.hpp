#pragma once

#include<utility>

#include"core/base.hpp"

namespace Arcadia
{
    struct Uuid
    {
    public:
        using value_type = std::uint64_t;
        using self_type = Uuid;
    public:
        static auto Zero() -> Uuid
        {
            return Uuid{ 0 };
        }

        Uuid():
            _Val(_NextVal++)
        {}
        Uuid(value_type val):
            _Val(val)
        {}

        [[nodiscard]]
        auto Get() const -> value_type
        {
            return _Val;
        }

        operator value_type() const
        {
            return Get();
        }
    private:
        static inline value_type _NextVal{ 1 };
        value_type _Val;
    };

    template<class Value>
    struct BasicIdentifiable: Arcadia::Noncopyable
    {
    public:
        using value_type = Value;
        using self_type = BasicIdentifiable<value_type>;
    public:
        BasicIdentifiable() = default;
        BasicIdentifiable(const value_type& val):
            _Value(val)
        {}
        BasicIdentifiable(value_type&& val):
            _Value(val)
        {}
        template<class ...Args>
        BasicIdentifiable(Args&& ...args) :
            _Value(std::forward<Args>(args)...)
        {}

        BasicIdentifiable(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;

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
        auto GetUuid() const -> const Arcadia::Uuid&
        {
            return _Uuid;
        }

        [[nodiscard]]
        auto GetValue() const -> const value_type&
        {
            return _Value;
        }

    private:
        Arcadia::Uuid _Uuid{};
        value_type _Value{};
    };
}

namespace std
{
    template<>
    struct hash<Arcadia::Uuid>
    {
        auto operator()(const Arcadia::Uuid& Uuid) const->std::size_t
        {
            return std::hash<Arcadia::Uuid::value_type>{}(Uuid);
        }
    };

    template<>
    struct std::formatter<Arcadia::Uuid>: std::formatter<std::string>
    {
        auto format(const Arcadia::Uuid& Uuid, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{}", Uuid.Get()),
                ctx
            );
        }
    };

    template<class Value>
    struct tuple_size<Arcadia::BasicIdentifiable<Value>>:
        std::integral_constant<std::size_t, 2>
    {};

    template<std::size_t Index, class Value>
    struct tuple_element<Index, Arcadia::BasicIdentifiable<Value>>:
        std::tuple_element<Index, std::tuple<Arcadia::Uuid, Value>>
    {};
}
