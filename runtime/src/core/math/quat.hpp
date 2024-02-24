#pragma once

#define GLM_FORCE_QUAT_DATA_WXYZ
#include"glm/gtc/quaternion.hpp"

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace arcadia
{
    namespace quat
    {
        ARCADIA_API constexpr auto create_zero() -> glm::quat
        {
            return glm::quat{};
        }
        ARCADIA_API constexpr auto create_identity() -> glm::quat
        {
            return glm::quat{ 1.f,.0f,.0f,.0f };
        }
    }
}
