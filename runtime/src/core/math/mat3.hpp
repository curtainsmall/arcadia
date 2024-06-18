#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace Mat3
{
    [[nodiscard]]
    ACDA_API constexpr auto Zero() -> glm::mat3
    {
        return glm::mat3{ .0f };
    }
    [[nodiscard]]
    ACDA_API constexpr auto Identity() -> glm::mat3
    {
        return glm::mat3{};
    }
}
