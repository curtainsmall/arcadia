#pragma once

#include"core/base.hpp"
#include"core/math/glm.hpp"

namespace arcadia
{
    namespace mat3
    {

        ARCADIA_API constexpr auto create_zero() -> glm::mat3
        {
            return glm::mat3{ .0f };
        }
        ARCADIA_API constexpr auto create_identity() -> glm::mat3
        {
            return glm::mat3{};
        }
    }
}
