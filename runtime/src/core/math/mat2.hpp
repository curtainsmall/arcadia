#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace Arcadia
{
    namespace GlmMat2
    {
        [[nodiscard]]
        ACDA_API constexpr auto CreateZero() -> glm::mat2
        {
            return glm::mat2{ .0f };
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateIdentity() -> glm::mat2
        {
            return glm::mat2{};
        }
    }
}