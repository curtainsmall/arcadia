#pragma once

#include<concepts>
#include<variant>

#include"core/base.hpp"
#include"core/util/template.hpp"

namespace arcadia
{
    template<
        class Ret,
        arcadia::instantiated_from<std::variant> Variant,
        class ...BranchFns
    >
    auto match(Variant& variant, BranchFns&& ...fns) -> Ret
    {
        return std::visit<Ret>(
            arcadia::overloaded{
                std::forward<BranchFns>(fns)...
            },
            variant
        );
    }

    template<
        class Ret,
        arcadia::instantiated_from<std::variant> Variant,
        class ...BranchFns
    >
    auto match(const Variant& variant, BranchFns&& ...fns) -> Ret
    {
        return std::visit<Ret>(
            arcadia::overloaded{
                std::forward<BranchFns>(fns)...
            },
            variant
        );
    }

    template<
        class Ret,
        class Case,
        std::convertible_to<Case> Cond,
        class ...Cases
    >
        requires (sizeof...(Cases) % 2 == 0)
    ARCADIA_API auto match(const Cond& cond, const Case& case_expr, const std::function<Ret()> case_fn, Cases&& ...cases) -> Ret
    {
        if constexpr(sizeof...(Cases) == 0)
        {
            return cond == case_expr ? case_fn() : Ret();
        }
        else
        {
            return cond == case_expr ? case_fn() : arcadia::match<Ret, Cond, Case>(cond, std::forward<Cases>(cases)...);
        }
    }

    template<
        class Ret,
        class Case,
        std::convertible_to<Case> Cond,
        class ...Cases
    >
        requires (sizeof...(Cases) % 2 == 0)
    ARCADIA_API auto match(const Cond& cond, const std::function<Ret()>& default_fn, const Case& case_expr, const std::function<Ret()> case_fn, Cases&& ...cases) -> Ret
    {
        if constexpr(sizeof...(Cases) == 0)
        {
            return cond == case_expr ? case_fn() : default_fn();
        }
        else
        {
            return cond == case_expr ? case_fn() : arcadia::match<Ret, Cond, Case>(cond, std::forward<Cases>(cases)...);
        }
    }
}
