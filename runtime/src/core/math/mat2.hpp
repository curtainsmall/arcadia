#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace mat2
{
    [[nodiscard]]
    ACDA_API constexpr auto zero() -> glm::mat2
    {
        return glm::mat2{ .0f };
    }
    [[nodiscard]]
    ACDA_API constexpr auto identity() -> glm::mat2
    {
        return glm::mat2{};
    }
}