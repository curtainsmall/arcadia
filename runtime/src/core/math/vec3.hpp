#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace arcadia
{

    namespace vec3
    {
        [[nodiscard]]
        ARCADIA_API inline auto to_json(const glm::vec3& vec) -> nlohmann::json
        {
            return nlohmann::json{
                    { "x",vec.x },
                    { "y",vec.y },
                    { "z",vec.z }
            };
        }
        [[nodiscard]]
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::vec3
        {
            return glm::vec3{
                json.at("x"),
                json.at("y"),
                json.at("z")
            };
        }

        [[nodiscard]]
        ARCADIA_API constexpr auto zero() -> glm::vec3
        {
            return glm::vec3{};
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto pos_unit_x() -> glm::vec3
        {
            return glm::vec3{ 1.f,.0f,.0f };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto pos_unit_y() -> glm::vec3
        {
            return glm::vec3{ .0f,1.f,.0f };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto pos_unit_z() -> glm::vec3
        {
            return glm::vec3{ .0f,.0f,1.f };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto neg_unit_x() -> glm::vec3
        {
            return -pos_unit_x();
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto neg_unit_y() -> glm::vec3
        {
            return -pos_unit_y();
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto neg_unit_z() -> glm::vec3
        {
            return -pos_unit_z();
        }

        /// @brief Normalize vector with one axis fixed
        /// @param vec Vector to normalize
        /// @param fixed_index Index of fixed axis, must be 0, 1 or 2
        /// @return Normalized vector
        [[nodiscard]]
        ARCADIA_API inline auto fixed_normalize(const glm::vec3& vec, glm::vec3::length_type fixed_index) -> glm::vec3
        {
            ARCADIA_ASSERT(fixed_index >= 0 && fixed_index < vec.length());

            auto fixed = vec[fixed_index];
            auto a = vec[(fixed_index + 1) % 3];
            auto b = vec[(fixed_index + 2) % 3];

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
            res[fixed_index] = fixed;
            res[(fixed_index + 1) % 3] = a;
            res[(fixed_index + 2) % 3] = b;
            return res;
        }
    }

    namespace dvec3
    {
        [[nodiscard]]
        ARCADIA_API inline auto to_json(const glm::dvec3& vec) -> nlohmann::json
        {
            return nlohmann::json{
                     { "x",vec.x },
                     { "y",vec.y },
                     { "z",vec.z }
            };
        }
        [[nodiscard]]
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::dvec3
        {
            return glm::dvec3{
                json.at("x"),
                json.at("y"),
                json.at("z")
            };
        }

        [[nodiscard]]
        ARCADIA_API constexpr auto zero() -> glm::dvec3
        {
            return glm::dvec3{};
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto pos_unit_x() -> glm::dvec3
        {
            return glm::dvec3{ 1.,.0,.0 };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto pos_unit_y() -> glm::dvec3
        {
            return glm::dvec3{ .0,1.,.0 };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto pos_unit_z() -> glm::dvec3
        {
            return glm::dvec3{ .0,.0,1. };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto neg_unit_x() -> glm::dvec3
        {
            return -pos_unit_x();
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto neg_unit_y() -> glm::dvec3
        {
            return -pos_unit_y();
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto neg_unit_z() -> glm::dvec3
        {
            return -pos_unit_z();
        }

        /// @brief Normalize vector with one axis fixed
        /// @param vec Vector to normalize
        /// @param fixed_index Index of fixed axis, must be 0, 1 or 2
        /// @return Normalized vector
        [[nodiscard]]
        ARCADIA_API inline auto fixed_normalize(const glm::dvec3& vec, glm::dvec3::length_type fixed_index) -> glm::dvec3
        {
            ARCADIA_ASSERT(fixed_index >= 0 && fixed_index < vec.length());

            auto fixed = vec[fixed_index];
            auto a = vec[(fixed_index + 1) % 3];
            auto b = vec[(fixed_index + 2) % 3];

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

            glm::dvec3 res{};
            res[fixed_index] = fixed;
            res[(fixed_index + 1) % 3] = a;
            res[(fixed_index + 2) % 3] = b;
            return res;
        }
    }

    namespace ivec3
    {
        [[nodiscard]]
        ARCADIA_API inline auto to_json(const glm::ivec3& vec) -> nlohmann::json
        {
            return nlohmann::json{
                     { "x",vec.x },
                     { "y",vec.y },
                     { "z",vec.z }
            };
        }
        [[nodiscard]]
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::ivec3
        {
            return glm::ivec3{
                json.at("x"),
                json.at("y"),
                json.at("z")
            };
        }
    }

    namespace uvec3
    {
        [[nodiscard]]
        ARCADIA_API inline auto to_json(const glm::uvec3& vec) -> nlohmann::json
        {
            return nlohmann::json{
                     { "x",vec.x },
                     { "y",vec.y },
                     { "z",vec.z }
            };
        }
        [[nodiscard]]
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::uvec3
        {
            return glm::uvec3{
                json.at("x"),
                json.at("y"),
                json.at("z")
            };
        }
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

    template<>
    struct std::formatter<glm::uvec3>: std::formatter<std::string>
    {
        auto format(const glm::uvec3& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2f}, {:.2f}, {:.2f}", vec.x, vec.y, vec.z),
                ctx
            );
        }
    };
}
