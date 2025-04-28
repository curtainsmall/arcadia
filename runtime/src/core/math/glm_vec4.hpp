#pragma once

#include "core/math/glm_header.hpp"
#include "core/nlohmann_json_header.hpp"
#include "platform/api_def.hpp"

namespace Arcadia
{
    namespace GlmVec4
    {
        [[nodiscard]]
        ACDA_API static inline auto ToJson(const glm::vec4& vec) -> nlohmann::json
        {
            return nlohmann::json::array(
                {
                    vec.x,
                    vec.y,
                    vec.z,
                    vec.w
                }
            );
        }
        [[nodiscard]]
        ACDA_API static inline auto FromJson(const nlohmann::json& json) -> glm::vec4
        {
            return glm::vec4(
                json[0],
                json[1],
                json[2],
                json[3]
            );
        }

        [[nodiscard]]
        ACDA_API constexpr auto CreateZero() -> glm::vec4
        {
            return glm::vec4{};
        }

        template<std::size_t Index>
            requires (Index >= 0) && (Index <= 3)
        [[nodiscard]]
        ACDA_API auto NormalizeFixedly(const glm::vec4& vec) -> glm::vec4
        {
            static_assert(Index >= 0 && Index < vec.length());

            auto fixed = vec[Index];
            auto a = vec[(Index + 1) % 4];
            auto b = vec[(Index + 2) % 4];
            auto c = vec[(Index + 3) % 4];

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

            glm::vec4 res{};
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
    class formatter<glm::vec4>: public std::formatter<std::string>
    {
    public:
        auto format(const glm::vec4& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2f}, {:.2f}, {:.2f}, {:.2f}", vec.x, vec.y, vec.z, vec.w),
                ctx
            );
        }
    };
}