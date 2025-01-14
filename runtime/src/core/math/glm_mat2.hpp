#pragma once

#include"platform/api_def.hpp"
#include"core/math/glm_header.hpp"
#include"core/math/glm_vec2.hpp"
#include"core/nlohmann_json_header.hpp"

namespace Arcadia
{
    namespace GlmMat2
    {
        [[nodiscard]]
        ACDA_API static inline auto ToJson(const glm::mat2& mat) -> nlohmann::json
        {
            return nlohmann::json::array(
                {
                    GlmVec2::ToJson(mat[0]),
                    GlmVec2::ToJson(mat[1])
                }
            );
        }

        [[nodiscard]]
        ACDA_API static inline auto FromJson(const nlohmann::json& json) -> glm::mat2
        {
            return glm::mat2(
                GlmVec2::FromJson(json[0]),
                GlmVec2::FromJson(json[1])
            );
        }

        [[nodiscard]]
        ACDA_API constexpr auto CreateZero() -> glm::mat2
        {
            return glm::mat2(.0f);
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateIdentity() -> glm::mat2
        {
            return glm::mat2{};
        }
    }
}