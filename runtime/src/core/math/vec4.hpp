#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace arcadia
{
    namespace vec4
    {
        ARCADIA_API extern auto to_json(const glm::vec4& vec) -> nlohmann::json;
        ARCADIA_API extern auto from_json(const nlohmann::json& json) -> glm::vec4;

        ARCADIA_API constexpr auto zero() -> glm::vec4
        {
            return glm::vec4{};
        }
    }
}

namespace std
{
    template<>
    struct std::formatter<glm::vec4>: std::formatter<std::string>
    {
        auto format(const glm::vec4& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2},{:.2},{:.2},{:.2}", vec.x, vec.y, vec.z, vec.w),
                ctx
            );
        }
    };
}
