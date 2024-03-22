#pragma once

#include"boost/math/special_functions/sign.hpp"

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

        /// @brief Normalize quaternion with one axis fixed
        /// @param vec Quaternion to normalize
        /// @param fixed_index Index of fixed axis, must be 0, 1, 2 or 3
        /// @return Normalized quaternion
        ARCADIA_API inline auto fixed_normalize(const glm::quat& quat, glm::quat::length_type fixed_index) -> glm::quat
        {
            ARCADIA_ASSERT(fixed_index >= 0 && fixed_index < quat.length());

            auto fixed = quat[fixed_index];
            auto a = quat[(fixed_index + 1) % 4];
            auto b = quat[(fixed_index + 2) % 4];
            auto c = quat[(fixed_index + 3) % 4];


            auto R = std::sqrt(1 - fixed * fixed); // Radius of target cicle
            auto r = std::sqrt(a * a + b * b + c * c); // Radius of the circle point (a,b,c) is on
            if(r != 0)
            {
                a = a * R / r;
                b = b * R / r;
                c = c * R / r;
            }
            else
            {
                a = R;
                b = 0;
                c = 0;
            }

            glm::quat res{};
            res[fixed_index] = fixed;
            res[(fixed_index + 1) % 4] = a;
            res[(fixed_index + 2) % 4] = b;
            res[(fixed_index + 3) % 4] = c;
            return res;
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
