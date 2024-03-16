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
            return uuid{};
        }

        inline uuid():
            _val(_next_val++)
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
            return std::hash<arcadia::uuid::value_type>{}(uuid);
        }
    };
}
