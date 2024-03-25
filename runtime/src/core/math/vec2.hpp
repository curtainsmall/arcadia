#pragma once

#include"boost/math/special_functions.hpp"

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace arcadia
{
    namespace vec2
    {
        [[nodiscard]]
        ARCADIA_API inline auto to_json(const glm::vec2& vec) -> nlohmann::json
        {
            return nlohmann::json{
                 { "x",vec.x },
                 { "y",vec.y }
            };
        }
        [[nodiscard]]
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::vec2
        {
            return glm::vec2{
                json.at("x"),
                json.at("y")
            };
        }

        [[nodiscard]]
        ARCADIA_API constexpr auto zero() -> glm::vec2
        {
            return glm::vec2{};
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto pos_unit_x() -> glm::vec2
        {
            return glm::vec2{ 1.f,.0f };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto pos_unit_y() -> glm::vec2
        {
            return glm::vec2{ .0f,1.f };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto neg_unit_x() -> glm::vec2
        {
            return -pos_unit_x();
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto neg_unit_y() -> glm::vec2
        {
            return -pos_unit_y();
        }

        /// @brief Normalize vector with one axis fixed
        /// @param vec Vector to normalize
        /// @param fixed_index Index of fixed axis, must be 0 or 1
        /// @return Normalized vector
        [[nodiscard]]
        ARCADIA_API inline auto fixed_normalize(const glm::vec2& vec, glm::vec2::length_type fixed_idx) -> glm::vec2
        {
            ARCADIA_ASSERT(fixed_idx >= 0 && vec.length());

            auto fixed = vec[fixed_idx];
            auto a = vec[1 - fixed_idx];

            if(a != 0)
            {
                a = boost::math::sign(a) * std::sqrt(1 - fixed * fixed);
            }

            glm::vec2 res{};
            res[fixed_idx] = fixed;
            res[1 - fixed_idx] = a;
            return res;
        }

    }

    namespace dvec2
    {
        [[nodiscard]]
        ARCADIA_API inline auto to_json(const glm::dvec2& vec) -> nlohmann::json
        {
            return nlohmann::json{
                    { "x",vec.x },
                    { "y",vec.y }
            };
        }
        [[nodiscard]]
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::dvec2
        {
            return glm::dvec2{
                json.at("x"),
                json.at("y")
            };
        }

        [[nodiscard]]
        ARCADIA_API constexpr auto zero() -> glm::dvec2
        {
            return glm::dvec2{};
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto pos_unit_x() -> glm::dvec2
        {
            return glm::dvec2{ 1., .0 };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto pos_unit_y() -> glm::dvec2
        {
            return glm::dvec2{ .0, 1. };
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto neg_unit_x() -> glm::dvec2
        {
            return -pos_unit_x();
        }
        [[nodiscard]]
        ARCADIA_API constexpr auto neg_unit_y() -> glm::dvec2
        {
            return -pos_unit_y();
        }

        /// @brief Normalize vector with one axis fixed
        /// @param vec Vector to normalize
        /// @param fixed_index Index of fixed axis, must be 0 or 1
        /// @return Normalized vector
        [[nodiscard]]
        ARCADIA_API inline auto fixed_normalize(const glm::dvec2& vec, glm::dvec2::length_type fixed_index) -> glm::dvec2
        {
            ARCADIA_ASSERT(fixed_index >= 0 && fixed_index < vec.length());

            auto fixed = vec[fixed_index];
            auto a = vec[1 - fixed_index];

            if(a != 0)
            {
                a = boost::math::sign(a) * std::sqrt(1 - fixed * fixed);
            }

            glm::dvec2 res{};
            res[fixed_index] = fixed;
            res[1 - fixed_index] = a;
            return res;
        }
    }


    namespace ivec2
    {

        [[nodiscard]]
        ARCADIA_API inline auto to_json(const glm::ivec2& vec) -> nlohmann::json
        {
            return nlohmann::json{
                {"x",vec.x},
                {"y",vec.y}
            };
        }
        [[nodiscard]]
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::ivec2
        {
            return glm::ivec2{
                    json.at("x"),
                    json.at("y")
            };
        }
    }

    namespace uvec2
    {
        [[nodiscard]]
        ARCADIA_API inline auto to_json(const glm::uvec2& vec) -> nlohmann::json
        {
            return nlohmann::json{
                {"x",vec.x},
                {"y",vec.y}
            };
        }
        [[nodiscard]]
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::uvec2
        {
            return glm::uvec2{
                    json.at("x"),
                    json.at("y")
            };
        }
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
    struct std::formatter<glm::dvec2>: std::formatter<std::string>
    {
        auto format(const glm::dvec2& vec, std::format_context& ctx) const
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

    template<>
    struct std::formatter<glm::uvec2>: std::formatter<std::string>
    {
        auto format(const glm::uvec2& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2f}, {:.2f}", vec.x, vec.y),
                ctx
            );
        }
    };
}
