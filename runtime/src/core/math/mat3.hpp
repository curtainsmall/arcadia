#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace mat3
{
    [[nodiscard]]
    ACDA_API constexpr auto zero() -> glm::mat3
    {
        return glm::mat3{ .0f };
    }
    [[nodiscard]]
    ACDA_API constexpr auto identity() -> glm::mat3
    {
        return glm::mat3{};
    }
}
