#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace Mat2
{
    [[nodiscard]]
    ARCADIA_API constexpr auto Zero() -> glm::mat2
    {
        return glm::mat2{ .0f };
    }
    [[nodiscard]]
    ARCADIA_API constexpr auto Identity() -> glm::mat2
    {
        return glm::mat2{};
    }
}