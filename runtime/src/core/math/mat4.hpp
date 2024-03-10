#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace arcadia
{
    namespace mat4
    {
        ARCADIA_API constexpr auto zero() -> glm::mat4
        {
            return glm::mat4{ .0f };
        }
        ARCADIA_API constexpr auto zero_affine() -> glm::mat4
        {
            return glm::mat4{
            glm::vec4{},
            glm::vec4{},
            glm::vec4{},
            glm::vec4{ .0f,.0f,.0f,1.f }
            };
        }
        ARCADIA_API constexpr auto identity() -> glm::mat4
        {
            return glm::mat4{};
        }
    }

}