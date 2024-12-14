#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace Arcadia
{
    namespace GlmMat3
    {
        [[nodiscard]]
        ACDA_API constexpr auto CreateZero() -> glm::mat3
        {
            return glm::mat3{ .0f };
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateIdentity() -> glm::mat3
        {
            return glm::mat3{};
        }
    }
}