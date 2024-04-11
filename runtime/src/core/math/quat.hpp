#pragma once

#include"boost/math/special_functions/sign.hpp"

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace Arcadia
{
    namespace Quat
    {
        [[nodiscard]]
        ARCADIA_API static inline auto ToJson(const glm::quat& quat) -> nlohmann::json
        {
            return nlohmann::json{
                { "w",quat.w },
                { "x",quat.x },
                { "y",quat.y },
                { "z",quat.z },
            };
        }
        [[nodiscard]]
        ARCADIA_API static inline auto FromJson(const nlohmann::json& json) -> glm::quat
        {
            return glm::quat{
                json.at("w"),
                json.at("x"),
                json.at("y"),
                json.at("z")
            };
        }

        [[nodiscard]]
        ARCADIA_API constexpr auto Zero() -> glm::quat
        {
            return glm::quat{};
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto Identity() -> glm::quat
        {
            return glm::quat{ 1.f,.0f,.0f,.0f };
        }

        /// @brief Normalize quaternion with one axis fixed
        /// @tparam Index Index of fixed axis, must be 0, 1, 2 or 3
        /// @param vec Quaternion to normalize
        /// @return Normalized quaternion
        template<std::size_t Index>
        [[nodiscard]]
        ARCADIA_API static inline auto FixedNormalize(const glm::quat& quat) -> glm::quat
        {
            static_assert(Index >= 0 && Index < 4);

            auto fixed = quat[Index];
            auto a = quat[(Index + 1) % 4];
            auto b = quat[(Index + 2) % 4];
            auto c = quat[(Index + 3) % 4];


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
            res[Index] = fixed;
            res[(Index + 1) % 4] = a;
            res[(Index + 2) % 4] = b;
            res[(Index + 3) % 4] = c;
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
                std::format("{:.2f},{:.2f},{:.2f},{:.2f}", quat.w, quat.x, quat.y, quat.z),
                ctx
            );
        }
    };
}
