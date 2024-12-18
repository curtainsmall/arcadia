#pragma once

#include<utility>
#include<variant>

namespace Arcadia
{
    template<typename T>
    class Nullable
    {
    public:
        using ValueType = T;
        using SelfType = Nullable<T>;
    public:
        Nullable() = default;
        Nullable(const ValueType& val) :
            _Value(val)
        {}
        Nullable(ValueType&& val) :
            _Value(std::move(val))
        {}
        template<typename ...Args>
        Nullable(Args&& ...args) :
            _Value(std::in_place_type<ValueType>, std::forward<Args>(args)...)
        {}

        Nullable(const SelfType&) = default;
        auto operator=(const SelfType&)->SelfType & = default;
        Nullable(SelfType&&) noexcept = default;
        auto operator=(SelfType&&) noexcept -> SelfType & = default;

        operator bool() const
        {
            return HasValue();
        }

        auto HasValue() const -> bool
        {
            return !IsNull();
        }

        auto IsNull() const -> bool
        {
            return std::holds_alternative<std::monostate>(_Value);
        }

        template<typename ...Args>
        auto EmplaceValue(Args&& ...args) -> ValueType&
        {
            _Value.emplace<ValueType>(std::forward<Args>(args)...);
            return std::get<ValueType>(_Value);
        }

        void RemoveValue()
        {
            _Value.emplace<std::monostate>();
        }

        auto GetValuePointer() -> ValueType*
        {
            return std::get_if<ValueType>(&_Value);
        }

        template<typename Ret>
        auto Match(
            const std::function<Ret(ValueType&)>& fn,
            const std::function<Ret()>& null_fn
        )
        {
            if(IsNull())
            {
                return null_fn();
            }
            else
            {
                return fn(std::get<ValueType>(_Value));
            }
        }
    private:
        std::variant<std::monostate, ValueType> _Value{};
    };
}