#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace arcadia
{
    namespace mat2
    {

        ARCADIA_API constexpr auto create_zero() -> glm::mat2
        {
            return glm::mat2{ .0f };
        }
        ARCADIA_API constexpr auto create_identity() -> glm::mat2
        {
            return glm::mat2{};
        }
    }

}