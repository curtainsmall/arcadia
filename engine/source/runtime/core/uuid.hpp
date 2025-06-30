#pragma once

#include <utility>

#include "core/noncopyable.hpp"
#include "platform/defines.hpp"

namespace Arcadia
{
    struct ACDA_API Uuid
    {
    public:
        using ValueType = std::uint64_t;
        using SelfType = Uuid;
    public:
        static auto CreateZero() -> Uuid;

        Uuid();
        Uuid(ValueType val);

        [[nodiscard]]
        auto GetValue() const->ValueType;

        operator ValueType() const;

    private:
        static inline ValueType _NextValue{ 1 };
        ValueType _Value;
    };
}

namespace std
{
    template<>
    struct ACDA_API hash<Arcadia::Uuid>
    {
    public:
        auto operator()(const Arcadia::Uuid& Uuid) const->std::size_t
        {
            return std::hash<Arcadia::Uuid::ValueType>()(Uuid);
        }
    };

    template<>
    struct ACDA_API formatter<Arcadia::Uuid>: public std::formatter<std::string>
    {
    public:
        auto format(const Arcadia::Uuid& Uuid, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{}", Uuid.GetValue()),
                ctx
            );
        }
    };
}