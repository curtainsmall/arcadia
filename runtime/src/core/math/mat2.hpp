#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"

namespace arcadia
{
    namespace mat2
    {
        [[nodiscard]]
        ARCADIA_API constexpr auto zero() -> glm::mat2
        {
            return glm::mat2{ .0f };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto identity() -> glm::mat2
        {
            return glm::mat2{};
        }
    }

    namespace dmat2
    {
        [[nodiscard]]
        ARCADIA_API constexpr auto zero() -> glm::dmat2
        {
            return glm::dmat2{ .0 };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto identity() -> glm::dmat2
        {
            return glm::dmat2{};
        }
    }

}