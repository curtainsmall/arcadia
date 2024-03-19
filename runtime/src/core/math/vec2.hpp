#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace arcadia
{
    namespace vec2
    {
        ARCADIA_API inline auto to_json(const glm::vec2& vec) -> nlohmann::json
        {
            return nlohmann::json{
                 { "x",vec.x },
                 { "y",vec.y }
            };
        }
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::vec2
        {
            return glm::vec2{
                json.at("x"),
                json.at("y")
            };
        }

        ARCADIA_API constexpr auto zero() -> glm::vec2
        {
            return glm::vec2{};
        }
        ARCADIA_API constexpr auto pos_unit_x() -> glm::vec2
        {
            return glm::vec2{ 1.f,.0f };
        }
        ARCADIA_API constexpr auto pos_unit_y() -> glm::vec2
        {
            return glm::vec2{ .0f,1.f };
        }
        ARCADIA_API constexpr auto neg_unit_x() -> glm::vec2
        {
            return -pos_unit_x();
        }
        ARCADIA_API constexpr auto neg_unit_y() -> glm::vec2
        {
            return -pos_unit_y();
        }
    }

    namespace dvec2
    {
        ARCADIA_API inline auto to_json(const glm::dvec2& vec) -> nlohmann::json
        {
            return nlohmann::json{
                    { "x",vec.x },
                    { "y",vec.y }
            };
        }
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::dvec2
        {
            return glm::dvec2{
                json.at("x"),
                json.at("y")
            };
        }

        ARCADIA_API constexpr auto zero() -> glm::dvec2
        {
            return glm::dvec2{};
        }
        ARCADIA_API constexpr auto pos_unit_x() -> glm::dvec2
        {
            return glm::dvec2{ 1., .0 };
        }
        ARCADIA_API constexpr auto pos_unit_y() -> glm::dvec2
        {
            return glm::dvec2{ .0, 1. };
        }
        ARCADIA_API constexpr auto neg_unit_x() -> glm::dvec2
        {
            return -pos_unit_x();
        }
        ARCADIA_API constexpr auto neg_unit_y() -> glm::dvec2
        {
            return -pos_unit_y();
        }
    }


    namespace ivec2
    {

        ARCADIA_API inline auto to_json(const glm::ivec2& vec) -> nlohmann::json
        {
            return nlohmann::json{
                {"x",vec.x},
                {"y",vec.y}
            };
        }
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
        ARCADIA_API inline auto to_json(const glm::uvec2& vec) -> nlohmann::json
        {
            return nlohmann::json{
                {"x",vec.x},
                {"y",vec.y}
            };
        }
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
                std::format("{:.2},{:.2}", vec.x, vec.y),
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
                std::format("{:.2},{:.2}", vec.x, vec.y),
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
                std::format("{:.2},{:.2}", vec.x, vec.y),
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
                std::format("{:.2},{:.2}", vec.x, vec.y),
                ctx
            );
        }
    };
}
