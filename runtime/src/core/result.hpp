#pragma once

#include<functional>
#include<variant>

#include"core/match.hpp"
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
        using SuccessType = Success;
        using ErrorTypePack = ParameterPack<Errors...>;

        using SelfType = Result<Success, Errors...>;

    public:
        Result() = delete;

        Result(const SuccessType& ok) :
            _Value(ok)
        {}
        Result(SuccessType&& ok) :
            _Value(std::move(ok))
        {}

        template<cTypeInParameterPack<ErrorTypePack> Error>
        Result(const Error& err) :
            _Value(err)
        {}
        template<cTypeInParameterPack<ErrorTypePack> Error>
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

        auto IsError() const -> bool
        {
            return !IsSuccess();
        }

        template<cTypeInParameterPack<ErrorTypePack> Error>
        auto IsError() const -> bool
        {
            return std::holds_alternative<Error>(_Value);
        }

        auto GetSuccessValuePointer() -> Success*
        {
            return std::get_if<Success>(&_Value);
        }

        template<cTypeInParameterPack<ErrorTypePack> Error>
        auto GetErrorValuePointer() -> Error*
        {
            return std::get_if<Error>(&_Value);
        }

        template<
            typename Ret,
            typename ErrorFunction,
            typename ...ErrorsFunctions
        >
        auto Match(
            const std::function<Ret(SuccessType&)>& success_fn,
            const ErrorFunction& error_fn,
            ErrorsFunctions&& ...error_fns
        )
        {
            return MatchVariant<Ret>(_Value, success_fn, error_fn, std::forward<ErrorsFunctions>(error_fns)...);
        }
    private:
        std::variant<Success, Errors...> _Value{};
    };
}