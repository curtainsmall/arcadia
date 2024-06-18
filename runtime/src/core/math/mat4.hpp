#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace Mat4
{
    [[nodiscard]]
    ARCADIA_API constexpr auto Zero() -> glm::mat4
    {
        return glm::mat4{ .0f };
    }
    [[nodiscard]]
    ARCADIA_API constexpr auto ZeroAffine() -> glm::mat4
    {
        return glm::mat4{
            glm::vec4{},
            glm::vec4{},
            glm::vec4{},
            glm::vec4{ .0f,.0f,.0f,1.f }
        };
    }
    [[nodiscard]]
    ARCADIA_API constexpr auto Identity() -> glm::mat4
    {
        return glm::mat4{};
    }
}