#pragma once

#include<functional>
#include<variant>

#include"core/noncopyable.hpp"
#include"core/parameter_pack.hpp"

namespace Arcadia
{
    template<
        typename Success,
        typename ...Errors
    >
    class Result: public Noncopyable
    {
    public:
        using SelfType = Result<Success, Errors...>;

    private:
        using _SuccessType = Success;
        using _ErrorTypes = ParameterPack<Errors...>;

    public:
        Result() = default;

        Result(const _SuccessType& ok) :
            _Value(ok)
        {}
        Result(_SuccessType&& ok) :
            _Value(std::move(ok))
        {}

        template<cTypeInParameterPack<_ErrorTypes> Error>
        Result(const Error& err) :
            _Value(err)
        {}
        template<cTypeInParameterPack<_ErrorTypes> Error>
        Result(Error&& err) :
            _Value(std::move(err))
        {}

        Result(SelfType&&) noexcept = default;
        auto operator=(SelfType&&) noexcept ->SelfType & = default;

        operator bool() const
        {
            return IsSuccess();
        }

        auto IsSuccess() const -> bool
        {
            return std::holds_alternative<Success>(_Value);
        }

        auto IsNull() const -> bool
        {
            return std::holds_alternative<std::monostate>(_Value);
        }

        auto IsError() const -> bool
        {
            return !IsSuccess() && !IsNull();
        }

        template<cTypeInParameterPack<_ErrorTypes> Error>
        auto IsError() const -> bool
        {
            return std::holds_alternative<Error>(_Value);
        }

        template<
            typename Ret,
            cTypeInParameterPack<_ErrorTypes> Error,
            typename ...Errors
        >
        auto Match(
            const std::function<Ret(_SuccessType&)>& success_fn,
            const std::function<Ret()>& null_fn,
            const std::function<Ret(Error&)>& error_fn,
            std::function<Ret(Errors&)>&& ...error_handlers
        )
        {
            if(IsSuccess() && success_fn)
            {
                return success_fn(std::get<_SuccessType>(_Value));
            }
            else if(IsNull() && null_fn)
            {
                return null_fn();
            }
            else if(IsError<Error>() && error_fn)
            {
                return error_fn(std::get<Error>(_Value));
            }
            else
            {
                if constexpr(sizeof...(error_handlers) > 0)
                {
                    return Match<Ret, Errors...>(success_fn, null_fn, std::forward<std::function<Ret(Errors&)>>(error_handlers)...);
                }
                else
                {
                    return Ret();
                }
            }
        }

    private:
        std::variant<std::monostate, Success, Errors...> _Value{};
    };
}