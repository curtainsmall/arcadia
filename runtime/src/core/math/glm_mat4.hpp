#pragma once

#include "platform/api_def.hpp"
#include "core/math/glm_header.hpp"
#include "core/math/glm_vec4.hpp"
#include "core/nlohmann_json_header.hpp"

namespace Arcadia
{
    namespace GlmMat4
    {
        [[nodiscard]]
        ACDA_API static inline auto ToJson(const glm::mat4& mat) -> nlohmann::json
        {
            return nlohmann::json::array(
                {
                    GlmVec4::ToJson(mat[0]),
                    GlmVec4::ToJson(mat[1]),
                    GlmVec4::ToJson(mat[2]),
                    GlmVec4::ToJson(mat[3])
                }
            );
        }

        [[nodiscard]]
        ACDA_API static inline auto FromJson(const nlohmann::json& json) -> glm::mat4
        {
            return glm::mat4(
                GlmVec4::FromJson(json[0]),
                GlmVec4::FromJson(json[1]),
                GlmVec4::FromJson(json[2]),
                GlmVec4::FromJson(json[3])
            );
        }

        [[nodiscard]]
        ACDA_API constexpr auto CreateZero() -> glm::mat4
        {
            return glm::mat4(.0f);
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateZeroAffine() -> glm::mat4
        {
            return glm::mat4(
                glm::vec4{},
                glm::vec4{},
                glm::vec4{},
                glm::vec4{ .0f,.0f,.0f,1.f }
            );
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateIdentity() -> glm::mat4
        {
            return glm::mat4{};
        }
    }
}