#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace arcadia
{

    namespace vec3
    {
        ARCADIA_API inline auto to_json(const glm::vec3& vec) -> nlohmann::json
        {
            return nlohmann::json{
                    { "x",vec.x },
                    { "y",vec.y },
                    { "z",vec.z }
            };
        }
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::vec3
        {
            glm::vec3 vec{};
            vec.x = json.at("x");
            vec.y = json.at("y");
            vec.z = json.at("z");
            return vec;
        }

        ARCADIA_API constexpr auto zero() -> glm::vec3
        {
            return glm::vec3{};
        }
        ARCADIA_API constexpr auto pos_unit_x() -> glm::vec3
        {
            return glm::vec3{ 1.f,.0f,.0f };
        }
        ARCADIA_API constexpr auto pos_unit_y() -> glm::vec3
        {
            return glm::vec3{ .0f,1.f,.0f };
        }
        ARCADIA_API constexpr auto pos_unit_z() -> glm::vec3
        {
            return glm::vec3{ .0f,.0f,1.f };
        }
        ARCADIA_API constexpr auto neg_unit_x() -> glm::vec3
        {
            return -pos_unit_x();
        }
        ARCADIA_API constexpr auto neg_unit_y() -> glm::vec3
        {
            return -pos_unit_y();
        }
        ARCADIA_API constexpr auto neg_unit_z() -> glm::vec3
        {
            return -pos_unit_z();
        }
    }

    namespace dvec3
    {
        ARCADIA_API inline auto to_json(const glm::dvec3& vec) -> nlohmann::json
        {
            return nlohmann::json{
                     { "x",vec.x },
                     { "y",vec.y },
                     { "z",vec.z }
            };
        }
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::dvec3
        {
            glm::dvec3 vec{};
            vec.x = json.at("x");
            vec.y = json.at("y");
            vec.z = json.at("z");
            return vec;
        }

        ARCADIA_API constexpr auto zero() -> glm::dvec3
        {
            return glm::dvec3{};
        }
        ARCADIA_API constexpr auto pos_unit_x() -> glm::dvec3
        {
            return glm::dvec3{ 1.,.0,.0 };
        }
        ARCADIA_API constexpr auto pos_unit_y() -> glm::dvec3
        {
            return glm::dvec3{ .0,1.,.0 };
        }
        ARCADIA_API constexpr auto pos_unit_z() -> glm::dvec3
        {
            return glm::dvec3{ .0,.0,1. };
        }
        ARCADIA_API constexpr auto neg_unit_x() -> glm::dvec3
        {
            return -pos_unit_x();
        }
        ARCADIA_API constexpr auto neg_unit_y() -> glm::dvec3
        {
            return -pos_unit_y();
        }
        ARCADIA_API constexpr auto neg_unit_z() -> glm::dvec3
        {
            return -pos_unit_z();
        }
    }

    namespace ivec3
    {
        ARCADIA_API inline auto to_json(const glm::ivec3& vec) -> nlohmann::json
        {
            return nlohmann::json{
                     { "x",vec.x },
                     { "y",vec.y },
                     { "z",vec.z }
            };
        }
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::ivec3
        {
            glm::ivec3 vec{};
            vec.x = json.at("x");
            vec.y = json.at("y");
            vec.z = json.at("z");
            return vec;
        }
    }

    namespace uvec3
    {
        ARCADIA_API inline auto to_json(const glm::uvec3& vec) -> nlohmann::json
        {
            return nlohmann::json{
                     { "x",vec.x },
                     { "y",vec.y },
                     { "z",vec.z }
            };
        }
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::uvec3
        {
            glm::uvec3 vec{};
            vec.x = json.at("x");
            vec.y = json.at("y");
            vec.z = json.at("z");
            return vec;
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
                std::format("{:.2},{:.2},{:.2}", vec.x, vec.y, vec.z),
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
                std::format("{:.2},{:.2},{:.2}", vec.x, vec.y, vec.z),
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
                std::format("{:.2},{:.2},{:.2}", vec.x, vec.y, vec.z),
                ctx
            );
        }
    };
}
