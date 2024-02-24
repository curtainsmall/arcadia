#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace arcadia
{
    namespace mat4
    {
        ARCADIA_API constexpr auto create_zero() -> glm::mat4
        {
            return glm::mat4{ .0f };
        }
        ARCADIA_API constexpr auto create_zero_affine() -> glm::mat4
        {
            return glm::mat4{
            glm::vec4{},
            glm::vec4{},
            glm::vec4{},
            glm::vec4{ .0f,.0f,.0f,1.f }
            };
        }
        ARCADIA_API constexpr auto create_identity() -> glm::mat4
        {
            return glm::mat4{};
        }
    }

    namespace detail
    {
        ARCADIA_API auto _transform_with_pivot_to_orignal_and_back(
            const glm::vec3& pivot,
            const glm::mat4& transform
        ) -> glm::mat4;
    }

    [[nodiscard]]
    ARCADIA_API auto gen_scaling_mat4(
        const glm::vec3& scale,
        const glm::vec3& pivot ={}
    ) -> glm::mat4;

    [[nodiscard]]
    ARCADIA_API auto gen_rotation_mat4(
        const glm::vec3& rotation,
        const glm::vec3& pivot ={}
    ) -> glm::mat4;

    [[nodiscard]]
    ARCADIA_API auto gen_position_mat4(
        const glm::vec3& position,
        const glm::vec3& pivot ={}
    ) -> glm::mat4;
}