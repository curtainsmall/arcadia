#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace arcadia
{
    namespace mat3
    {
        [[nodiscard]]
        ARCADIA_API constexpr auto zero() -> glm::mat3
        {
            return glm::mat3{ .0f };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto identity() -> glm::mat3
        {
            return glm::mat3{};
        }
    }

    namespace dmat3
    {

        [[nodiscard]]
        ARCADIA_API constexpr auto zero() -> glm::dmat3
        {
            return glm::dmat3{ .0 };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto identity() -> glm::dmat3
        {
            return glm::dmat3{};
        }
    }
}
