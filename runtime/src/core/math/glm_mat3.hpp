#pragma once

#include"platform/api_def.hpp"
#include"core/math/glm_header.hpp"
#include"core/math/glm_vec3.hpp"
#include"core/nlohmann_json_header.hpp"

namespace Arcadia
{
    namespace GlmMat3
    {
        [[nodiscard]]
        ACDA_API static inline auto ToJson(const glm::mat3& mat) -> nlohmann::json
        {
            return nlohmann::json::array(
                {
                    GlmVec3::ToJson(mat[0]),
                    GlmVec3::ToJson(mat[1])
                }
            );
        }

        [[nodiscard]]
        ACDA_API static inline auto FromJson(const nlohmann::json& json) -> glm::mat3
        {
            return glm::mat3(
                GlmVec3::FromJson(json[0]),
                GlmVec3::FromJson(json[1]),
                GlmVec3::FromJson(json[2])
            );
        }

        [[nodiscard]]
        ACDA_API constexpr auto CreateZero() -> glm::mat3
        {
            return glm::mat3(.0f);
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateIdentity() -> glm::mat3
        {
            return glm::mat3{};
        }
    }
}