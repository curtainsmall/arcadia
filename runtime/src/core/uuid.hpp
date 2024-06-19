#pragma once

#include<utility>

#include"core/base.hpp"

struct Uuid
{
public:
    using value_type = std::uint64_t;
    using self_type = Uuid;
public:
    static auto zero() -> Uuid
    {
        return Uuid{ 0 };
    }

    Uuid():
        _val(_next_val++)
    {}
    Uuid(value_type val):
        _val(val)
    {}

    [[nodiscard]]
    auto value() const -> value_type
    {
        return _val;
    }

    operator value_type() const
    {
        return value();
    }
private:
    static inline value_type _next_val{ 1 };
    value_type _val;
};

template<class Value>
struct BasicIdentifiable: Noncopyable
{
public:
    using value_type = Value;
    using self_type = BasicIdentifiable<value_type>;
public:
    BasicIdentifiable() = default;
    BasicIdentifiable(const value_type& val):
        _value(val)
    {}
    BasicIdentifiable(value_type&& val):
        _value(val)
    {}
    template<class ...Args>
    BasicIdentifiable(Args&& ...args) :
        _value(std::forward<Args>(args)...)
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
            return uuid();
        }
        else
        {
            return value();
        }
    }

    [[nodiscard]]
    auto uuid() const -> const Uuid&
    {
        return _uuid;
    }

    [[nodiscard]]
    auto value() const -> const value_type&
    {
        return _value;
    }

private:
    Uuid _uuid{};
    value_type _value{};
};

namespace std
{
    template<>
    struct hash<Uuid>
    {
        auto operator()(const Uuid& Uuid) const->std::size_t
        {
            return std::hash<Uuid::value_type>{}(Uuid);
        }
    };

    template<>
    struct std::formatter<Uuid>: std::formatter<std::string>
    {
        auto format(const Uuid& Uuid, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{}", Uuid.value()),
                ctx
            );
        }
    };

    template<class Value>
    struct tuple_size<BasicIdentifiable<Value>>:
        std::integral_constant<std::size_t, 2>
    {};

    template<std::size_t Index, class Value>
    struct tuple_element<Index, BasicIdentifiable<Value>>:
        std::tuple_element<Index, std::tuple<Uuid, Value>>
    {};
}
