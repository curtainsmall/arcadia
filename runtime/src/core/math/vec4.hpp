#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace arcadia
{
    namespace vec4
    {
        ARCADIA_API constexpr auto create_zero() -> glm::vec4
        {
            return glm::vec4{};
        }
    }
}
