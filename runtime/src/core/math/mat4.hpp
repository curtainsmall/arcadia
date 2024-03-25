#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace arcadia
{
    namespace mat4
    {
        [[nodiscard]]
        ARCADIA_API constexpr auto zero() -> glm::mat4
        {
            return glm::mat4{ .0f };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto zero_affine() -> glm::mat4
        {
            return glm::mat4{
                glm::vec4{},
                glm::vec4{},
                glm::vec4{},
                glm::vec4{ .0f,.0f,.0f,1.f }
            };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto identity() -> glm::mat4
        {
            return glm::mat4{};
        }
    }

    namespace dmat4
    {
        [[nodiscard]]
        ARCADIA_API constexpr auto zero() -> glm::dmat4
        {
            return glm::dmat4{ .0f };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto zero_affine() -> glm::dmat4
        {
            return glm::dmat4{
                glm::dvec4{},
                glm::dvec4{},
                glm::dvec4{},
                glm::dvec4{ .0,.0,.0,1. }
            };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto identity() -> glm::dmat4
        {
            return glm::dmat4{};
        }
    }

}