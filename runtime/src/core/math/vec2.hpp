#pragma once

#include"boost/math/special_functions.hpp"

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace vec2
{
    [[nodiscard]]
    ACDA_API static inline auto to_json(const glm::vec2& vec) -> nlohmann::json
    {
        return nlohmann::json{
             { "x",vec.x },
             { "y",vec.y }
        };
    }
    [[nodiscard]]
    ACDA_API static inline auto from_json(const nlohmann::json& json) -> glm::vec2
    {
        return glm::vec2{
            json.at("x"),
            json.at("y")
        };
    }

    [[nodiscard]]
    ACDA_API constexpr auto zero() -> glm::vec2
    {
        return glm::vec2{};
    }
    [[nodiscard]]
    ACDA_API constexpr auto pos_x() -> glm::vec2
    {
        return glm::vec2{ 1.f,.0f };
    }
    [[nodiscard]]
    ACDA_API constexpr auto pos_y() -> glm::vec2
    {
        return glm::vec2{ .0f,1.f };
    }
    [[nodiscard]]
    ACDA_API constexpr auto neg_x() -> glm::vec2
    {
        return -pos_x();
    }
    [[nodiscard]]
    ACDA_API constexpr auto neg_y() -> glm::vec2
    {
        return -pos_y();
    }

    /// @brief Normalize vector with one axis fixed
    /// @tparam Index Index of fixed axis, must be 0 or 1
    /// @param vec Vector to normalize
    /// @return Normalized vector
    template<size_t Index>
    [[nodiscard]]
    ACDA_API static inline auto fixed_normalized(const glm::vec2& vec) -> glm::vec2
    {
        static_assert(Index >= 0 && Index < 2);

        auto fixed = vec[Index];
        auto a = vec[1 - Index];

        if(a != 0)
        {
            a = boost::math::sign(a) * std::sqrt(1 - fixed * fixed);
        }

        glm::vec2 res{};
        res[Index] = fixed;
        res[1 - Index] = a;
        return res;
    }

}

namespace ivec2
{

    [[nodiscard]]
    ACDA_API static inline auto to_json(const glm::ivec2& vec) -> nlohmann::json
    {
        return nlohmann::json{
            {"x",vec.x},
            {"y",vec.y}
        };
    }
    [[nodiscard]]
    ACDA_API static inline auto from_json(const nlohmann::json& json) -> glm::ivec2
    {
        return glm::ivec2{
                json.at("x"),
                json.at("y")
        };
    }
}

namespace std
{
    template<>
    struct std::formatter<glm::vec2>: std::formatter<std::string>
    {
        auto format(const glm::vec2& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2f}, {:.2f}", vec.x, vec.y),
                ctx
            );
        }
    };

    template<>
    struct std::formatter<glm::ivec2>: std::formatter<std::string>
    {
        auto format(const glm::ivec2& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2f}, {:.2f}", vec.x, vec.y),
                ctx
            );
        }
    };
}
