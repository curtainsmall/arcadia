#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace Arcadia
{
    namespace Mat4
    {
        [[nodiscard]]
        ACDA_API constexpr auto CreateZero() -> glm::mat4
        {
            return glm::mat4{ .0f };
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateZeroAffine() -> glm::mat4
        {
            return glm::mat4{
                glm::vec4{},
                glm::vec4{},
                glm::vec4{},
                glm::vec4{ .0f,.0f,.0f,1.f }
            };
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateIdentity() -> glm::mat4
        {
            return glm::mat4{};
        }
    }
}