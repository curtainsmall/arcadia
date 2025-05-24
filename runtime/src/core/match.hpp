#pragma once

#include <functional>
#include <variant>

#include "core/concept.hpp"
#include "platform/api_def.hpp"

namespace Arcadia
{
    template<class ...Fns>
    struct OverloadedFunctionsWrapper: public Fns...
    {
    public:
        using Fns::operator()...;
    };

    template<
        class Ret,
        Concepts::InstantiatedFrom<std::variant> Variant,
        class ...BranchFns
    >
    [[nodiscard]]
    ACDA_API auto MatchVariant(Variant& variant, BranchFns&& ...fns) -> Ret
    {
        return std::visit<Ret>(
            OverloadedFunctionsWrapper{
                std::forward<BranchFns>(fns)...
            },
            variant
        );
    }

    template<
        class Ret,
        Concepts::InstantiatedFrom<std::variant> Variant,
        class ...BranchFns
    >
    [[nodiscard]]
    ACDA_API auto MatchVariant(const Variant& variant, BranchFns&& ...fns) -> Ret
    {
        return std::visit<Ret>(
            OverloadedFunctionsWrapper{
                std::forward<BranchFns>(fns)...
            },
            variant
        );
    }

    template<
        class Ret,
        class Cond,
        std::convertible_to<Cond> Case,
        class ...Cases
    >
        requires (sizeof...(Cases) % 2 == 0)
    [[nodiscard]]
    ACDA_API auto Match(const Cond& cond, const Case& case_expr, const std::function<Ret()>& case_fn, Cases&& ...cases) -> Ret
    {
        if constexpr(sizeof...(Cases) == 0)
        {
            return cond == case_expr ? case_fn() : Ret();
        }
        else
        {
            return cond == case_expr ? case_fn() : Match<Ret>(cond, std::forward<Cases>(cases)...);
        }
    }

    template<
        class Ret,
        class Cond,
        std::convertible_to<Cond> Case,
        class ...Cases
    >
        requires (sizeof...(Cases) % 2 == 0)
    [[nodiscard]]
    ACDA_API auto Match(const Cond& cond, const std::function<Ret()>& default_fn, const Case& case_expr, const std::function<Ret()>& case_fn, Cases&& ...cases) -> Ret
    {
        if constexpr(sizeof...(Cases) == 0)
        {
            return cond == case_expr ? case_fn() : default_fn();
        }
        else
        {
            return cond == case_expr ? case_fn() : Match<Ret>(cond, std::forward<Cases>(cases)...);
        }
    }

    template<
        class Ret,
        class Cond,
        std::convertible_to<Cond> Case,
        class ...Cases
    >
        requires (sizeof...(Cases) % 2 == 0)
    [[nodiscard]]
    ACDA_API auto Match(const Cond& cond, const Case& case_expr, const Ret& case_res, Cases&& ...cases) -> Ret
    {
        if constexpr(sizeof...(Cases) == 0)
        {
            return cond == case_expr ? case_res : Ret();
        }
        else
        {
            return cond == case_expr ? case_res : Match<Ret>(cond, std::forward<Cases>(cases)...);
        }
    }

    template<
        class Ret,
        class Cond,
        std::convertible_to<Cond> Case,
        class ...Cases
    >
        requires (sizeof...(Cases) % 2 == 0)
    [[nodiscard]]
    ACDA_API auto Match(const Cond& cond, const Ret& default_res, const Case& case_expr, const Ret& case_res, Cases&& ...cases) -> Ret
    {
        if constexpr(sizeof...(Cases) == 0)
        {
            return cond == case_expr ? case_res : default_res;
        }
        else
        {
            return cond == case_expr ? case_res : Match<Ret>(cond, std::forward<Cases>(cases)...);
        }
    }
}