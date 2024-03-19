#pragma once

#define GLM_FORCE_QUAT_DATA_WXYZ
#include"glm/gtc/quaternion.hpp"

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace arcadia
{
    namespace quat
    {
        ARCADIA_API inline auto to_json(const glm::quat& quat) -> nlohmann::json
        {
            return nlohmann::json{
                { "w",quat.w },
                { "x",quat.x },
                { "y",quat.y },
                { "z",quat.z },
            };
        }
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::quat
        {
            return glm::quat{
                json.at("w"),
                json.at("x"),
                json.at("y"),
                json.at("z")
            };
        }

        ARCADIA_API constexpr auto zero() -> glm::quat
        {
            return glm::quat{};
        }
        ARCADIA_API constexpr auto identity() -> glm::quat
        {
            return glm::quat{ 1.f,.0f,.0f,.0f };
        }
    }
}

namespace std
{
    template<>
    struct std::formatter<glm::quat>: std::formatter<std::string>
    {
        auto format(const glm::quat& quat, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2},{:.2},{:.2},{:.2}", quat.w, quat.x, quat.y, quat.z),
                ctx
            );
        }
    };
}
