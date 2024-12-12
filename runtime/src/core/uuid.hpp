#pragma once

#include<utility>

#include"core/base.hpp"

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

    template<typename Value>
    class BasicIdentifiable: public Noncopyable
    {
    public:
        using ValueType = Value;
        using SelfType = BasicIdentifiable<ValueType>;
    public:
        BasicIdentifiable() = default;
        BasicIdentifiable(const ValueType& val) :
            _Value(val)
        {}
        BasicIdentifiable(ValueType&& val) :
            _Value(val)
        {}
        template<typename ...Args>
        BasicIdentifiable(Args&& ...args) :
            _Value(std::forward<Args>(args)...)
        {}

        BasicIdentifiable(SelfType&&) noexcept = default;
        auto operator=(SelfType&&) noexcept -> SelfType & = default;

        template<size_t Index>
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
    template<>
    class hash<Arcadia::Uuid>
    {
    public:
        auto operator()(const Arcadia::Uuid& Uuid) const->size_t
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

    template<typename Value>
    class tuple_size<Arcadia::BasicIdentifiable<Value>>:
        public std::integral_constant<size_t, 2>
    {};

    template<size_t Index, typename Value>
    class tuple_element<Index, Arcadia::BasicIdentifiable<Value>>:
        public std::tuple_element<Index, std::tuple<Arcadia::Uuid, Value>>
    {};
}