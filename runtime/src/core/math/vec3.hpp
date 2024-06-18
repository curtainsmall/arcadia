#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace Vec3
{
    [[nodiscard]]
    ARCADIA_API static inline auto ToJson(const glm::vec3& vec) -> nlohmann::json
    {
        return nlohmann::json{
                { "x",vec.x },
                { "y",vec.y },
                { "z",vec.z }
        };
    }
    [[nodiscard]]
    ARCADIA_API static inline auto FromJson(const nlohmann::json& json) -> glm::vec3
    {
        return glm::vec3{
            json.at("x"),
            json.at("y"),
            json.at("z")
        };
    }

    [[nodiscard]]
    ARCADIA_API constexpr auto Zero() -> glm::vec3
    {
        return glm::vec3{};
    }
    [[nodiscard]]
    ARCADIA_API constexpr auto PosX() -> glm::vec3
    {
        return glm::vec3{ 1.f,.0f,.0f };
    }
    [[nodiscard]]
    ARCADIA_API constexpr auto PosY() -> glm::vec3
    {
        return glm::vec3{ .0f,1.f,.0f };
    }
    [[nodiscard]]
    ARCADIA_API constexpr auto PosZ() -> glm::vec3
    {
        return glm::vec3{ .0f,.0f,1.f };
    }
    [[nodiscard]]
    ARCADIA_API constexpr auto NegX() -> glm::vec3
    {
        return -PosX();
    }
    [[nodiscard]]
    ARCADIA_API constexpr auto NegY() -> glm::vec3
    {
        return -PosY();
    }
    [[nodiscard]]
    ARCADIA_API constexpr auto NegZ() -> glm::vec3
    {
        return -PosZ();
    }

    /// @brief Normalize vector with one axis fixed
    /// @tparam Index Index of fixed axis, must be 0, 1 or 2
    /// @param vec Vector to normalize
    /// @return Normalized vector
    template<std::size_t Index>
    [[nodiscard]]
    ARCADIA_API static inline auto FixedNormalized(const glm::vec3& vec) -> glm::vec3
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

namespace IVec3
{
    [[nodiscard]]
    ARCADIA_API static inline auto ToJson(const glm::ivec3& vec) -> nlohmann::json
    {
        return nlohmann::json{
                 { "x",vec.x },
                 { "y",vec.y },
                 { "z",vec.z }
        };
    }
    [[nodiscard]]
    ARCADIA_API static inline auto FromJson(const nlohmann::json& json) -> glm::ivec3
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
