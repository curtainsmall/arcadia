#pragma once

#include"boost/math/special_functions.hpp"

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace Vec2
{
    [[nodiscard]]
    ARCADIA_API static inline auto ToJson(const glm::vec2& vec) -> nlohmann::json
    {
        return nlohmann::json{
             { "x",vec.x },
             { "y",vec.y }
        };
    }
    [[nodiscard]]
    ARCADIA_API static inline auto FromJson(const nlohmann::json& json) -> glm::vec2
    {
        return glm::vec2{
            json.at("x"),
            json.at("y")
        };
    }

    [[nodiscard]]
    ARCADIA_API constexpr auto Zero() -> glm::vec2
    {
        return glm::vec2{};
    }
    [[nodiscard]]
    ARCADIA_API constexpr auto PosX() -> glm::vec2
    {
        return glm::vec2{ 1.f,.0f };
    }
    [[nodiscard]]
    ARCADIA_API constexpr auto PosY() -> glm::vec2
    {
        return glm::vec2{ .0f,1.f };
    }
    [[nodiscard]]
    ARCADIA_API constexpr auto NegX() -> glm::vec2
    {
        return -PosX();
    }
    [[nodiscard]]
    ARCADIA_API constexpr auto NegY() -> glm::vec2
    {
        return -PosY();
    }

    /// @brief Normalize vector with one axis fixed
    /// @tparam Index Index of fixed axis, must be 0 or 1
    /// @param vec Vector to normalize
    /// @return Normalized vector
    template<std::size_t Index>
    [[nodiscard]]
    ARCADIA_API static inline auto FixedNormalize(const glm::vec2& vec) -> glm::vec2
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

namespace IVec2
{

    [[nodiscard]]
    ARCADIA_API static inline auto ToJson(const glm::ivec2& vec) -> nlohmann::json
    {
        return nlohmann::json{
            {"x",vec.x},
            {"y",vec.y}
        };
    }
    [[nodiscard]]
    ARCADIA_API static inline auto FromJson(const nlohmann::json& json) -> glm::ivec2
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
