#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace vec3
{
    [[nodiscard]]
    ACDA_API static inline auto to_json(const glm::vec3& vec) -> nlohmann::json
    {
        return nlohmann::json{
                { "x",vec.x },
                { "y",vec.y },
                { "z",vec.z }
        };
    }
    [[nodiscard]]
    ACDA_API static inline auto from_json(const nlohmann::json& json) -> glm::vec3
    {
        return glm::vec3{
            json.at("x"),
            json.at("y"),
            json.at("z")
        };
    }

    [[nodiscard]]
    ACDA_API constexpr auto zero() -> glm::vec3
    {
        return glm::vec3{};
    }
    [[nodiscard]]
    ACDA_API constexpr auto pos_x() -> glm::vec3
    {
        return glm::vec3{ 1.f,.0f,.0f };
    }
    [[nodiscard]]
    ACDA_API constexpr auto pos_y() -> glm::vec3
    {
        return glm::vec3{ .0f,1.f,.0f };
    }
    [[nodiscard]]
    ACDA_API constexpr auto pos_z() -> glm::vec3
    {
        return glm::vec3{ .0f,.0f,1.f };
    }
    [[nodiscard]]
    ACDA_API constexpr auto neg_x() -> glm::vec3
    {
        return -pos_x();
    }
    [[nodiscard]]
    ACDA_API constexpr auto neg_y() -> glm::vec3
    {
        return -pos_y();
    }
    [[nodiscard]]
    ACDA_API constexpr auto neg_z() -> glm::vec3
    {
        return -pos_z();
    }

    /// @brief Normalize vector with one axis fixed
    /// @tparam Index Index of fixed axis, must be 0, 1 or 2
    /// @param vec Vector to normalize
    /// @return Normalized vector
    template<std::size_t Index>
    [[nodiscard]]
    ACDA_API static inline auto fixed_normalized(const glm::vec3& vec) -> glm::vec3
    {
        static_assert(Index >= 0 && Index < 3);

        auto fixed = vec[Index];
        auto a = vec[(Index + 1) % 3];
        auto b = vec[(Index + 2) % 3];

        auto R = std::sqrt(1 - fixed * fixed); // Radius of target cicle
        auto r = std::sqrt(a * a + b * b); // Radius of the circle point (a,b) is on
        if(r != 0)
        {
            a = a * R / r;
            b = b * R / r;
        }
        else
        {
            a = R;
            b = 0;
        }

        glm::vec3 res{};
        res[Index] = fixed;
        res[(Index + 1) % 3] = a;
        res[(Index + 2) % 3] = b;
        return res;
    }
}

namespace ivec3
{
    [[nodiscard]]
    ACDA_API static inline auto to_json(const glm::ivec3& vec) -> nlohmann::json
    {
        return nlohmann::json{
                 { "x",vec.x },
                 { "y",vec.y },
                 { "z",vec.z }
        };
    }
    [[nodiscard]]
    ACDA_API static inline auto from_json(const nlohmann::json& json) -> glm::ivec3
    {
        return glm::ivec3{
            json.at("x"),
            json.at("y"),
            json.at("z")
        };
    }
}

namespace std
{
    template<>
    struct std::formatter<glm::vec3>: std::formatter<std::string>
    {
        auto format(const glm::vec3& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2f}, {:.2f}, {:.2f}", vec.x, vec.y, vec.z),
                ctx
            );
        }
    };

    template<>
    struct std::formatter<glm::ivec3>: std::formatter<std::string>
    {
        auto format(const glm::ivec3& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2f}, {:.2f}, {:.2f}", vec.x, vec.y, vec.z),
                ctx
            );
        }
    };
}
