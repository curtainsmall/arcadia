#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace vec4
{
    [[nodiscard]]
    ACDA_API static inline auto to_json(const glm::vec4& vec) -> nlohmann::json
    {
        return nlohmann::json{
            {"x",vec.x},
            {"y",vec.y},
            {"z",vec.z},
            {"w",vec.w}
        };
    }
    [[nodiscard]]
    ACDA_API static inline auto from_json(const nlohmann::json& json) -> glm::vec4
    {
        return glm::vec4{
            json.at("x"),
            json.at("y"),
            json.at("z"),
            json.at("w")
        };
    }

    [[nodiscard]]
    ACDA_API constexpr auto zero() -> glm::vec4
    {
        return glm::vec4{};
    }

    /// @brief Normalize vector with one axis fixed
    /// @tparam Index Index of fixed axis, must be 0, 1, 2 or 3
    /// @param vec Vector to normalize
    /// @return Normalized vector
    template<size_t Index>
    [[nodiscard]]
    ACDA_API auto normalize_fixedly(const glm::vec4& vec) -> glm::vec4
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

namespace std
{
    template<>
    struct std::formatter<glm::vec4>: std::formatter<std::string>
    {
        auto format(const glm::vec4& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2f}, {:.2f}, {:.2f}, {:.2f}", vec.x, vec.y, vec.z, vec.w),
                ctx
            );
        }
    };
}
