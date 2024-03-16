#pragma once

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
        ~uuid() = default;


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

    struct ARCADIA_API identifiable_base
    {
    public:
        using self_type = identifiable_base;
    public:
        identifiable_base() = default;

        [[nodiscard]]
        inline auto get_uuid() const -> arcadia::uuid
        {
            return _uuid;
        }

    private:
        arcadia::uuid _uuid{};
    };

    template<class ...Types>
    using identifiable_tuple = std::tuple<arcadia::uuid, Types...>;
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
}
