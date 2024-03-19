#pragma once

#include<utility>

#include"core/base.hpp"

namespace arcadia
{
    struct ARCADIA_API uuid
    {
    public:
        using value_type = std::uint64_t;
        using self_type = uuid;
    public:
        static inline auto zero() -> uuid
        {
            return uuid{ 0 };
        }

        inline uuid():
            _val(_next_val++)
        {}
        inline uuid(value_type val) :
            _val(val)
        {}

        [[nodiscard]]
        auto get() const -> value_type
        {
            return _val;
        }

        operator value_type() const
        {
            return get();
        }
    private:
        static inline value_type _next_val{ 1 };
        value_type _val;
    };

    template<class Value>
    struct ARCADIA_API basic_identifiable: arcadia::noncopyable
    {
    public:
        using value_type = Value;
        using self_type = basic_identifiable<value_type>;
    public:
        basic_identifiable() = default;
        basic_identifiable(const value_type& val):
            _value(val)
        {}
        basic_identifiable(value_type&& val):
            _value(val)
        {}
        template<class ...Args>
        basic_identifiable(Args&& ...args) :
            _value(std::forward<Args>(args)...)
        {}

        basic_identifiable(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;

        template<std::size_t Index>
        [[nodiscard]]
        auto get() const -> const auto&
        {
            static_assert(Index == 0 || Index == 1, "Invalid index");

            if constexpr(Index == 0)
            {
                return get_uuid();
            }
            else
            {
                return get_value();
            }
        }

        [[nodiscard]]
        inline auto get_uuid() const -> const arcadia::uuid&
        {
            return _uuid;
        }

        [[nodiscard]]
        inline auto get_value() const -> const value_type&
        {
            return _value;
        }
    private:
        arcadia::uuid _uuid{};
        value_type _value{};
    };
}

namespace std
{
    template<>
    struct hash<arcadia::uuid>
    {
        auto operator()(const arcadia::uuid& uuid) const->std::size_t
        {
            return std::hash<arcadia::uuid::value_type>{}(uuid);
        }
    };

    template<>
    struct std::formatter<arcadia::uuid>: std::formatter<std::string>
    {
        auto format(const arcadia::uuid& uuid, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{}", uuid.get()),
                ctx
            );
        }
    };

    template<class Value>
    struct tuple_size<arcadia::basic_identifiable<Value>>:
        std::integral_constant<std::size_t, 2>
    {};

    template<std::size_t Index, class Value>
    struct tuple_element<Index, arcadia::basic_identifiable<Value>>:
        std::tuple_element<Index, std::tuple<arcadia::uuid, Value>>
    {};
}
