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
        static inline auto next_val() -> uuid
        {
            return uuid{ _next_val++ };
        }
        static inline auto zero() -> uuid
        {
            return uuid{};
        }

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
        uuid() = default;
        uuid(value_type val):
            _val(val)
        {}
    private:
        static inline value_type _next_val{ 1 };
        value_type _val{ 0 };
    };
}

namespace std
{
    template<>
    struct hash<arcadia::uuid>
    {
        auto operator()(const arcadia::uuid& uuid) const->std::size_t
        {
            return uuid;
        }
    };
}
