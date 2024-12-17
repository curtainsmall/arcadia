#pragma once

#include<utility>

#include"core/noncopyable.hpp"
#include"platform/api_def.hpp"

namespace Arcadia
{
    class Uuid
    {
    public:
        using ValueType = std::uint64_t;
        using SelfType = Uuid;
    public:
        static auto CreateZero() -> Uuid
        {
            return Uuid{ 0 };
        }

        Uuid() :
            _Value(_NextValue++)
        {}
        Uuid(ValueType val) :
            _Value(val)
        {}

        [[nodiscard]]
        auto GetValue() const -> ValueType
        {
            return _Value;
        }

        operator ValueType() const
        {
            return GetValue();
        }
    private:
        static inline ValueType _NextValue{ 1 };
        ValueType _Value;
    };
}

namespace std
{
    template<>
    class hash<Arcadia::Uuid>
    {
    public:
        auto operator()(const Arcadia::Uuid& Uuid) const->std::size_t
        {
            return std::hash<Arcadia::Uuid::ValueType>()(Uuid);
        }
    };

    template<>
    class formatter<Arcadia::Uuid>: public std::formatter<std::string>
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