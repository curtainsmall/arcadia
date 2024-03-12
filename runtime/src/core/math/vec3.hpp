#pragma once

#include"flatbuffers_generated/math_generated.h"

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace arcadia
{

    namespace vec3
    {
        using serialization_type = arcadia::serialization::vec3;

        extern auto to_flatbuffers(const glm::vec3& vec) -> serialization_type;
        extern auto from_flatbuffers(const serialization_type& flat_vec) -> glm::vec3;

        ARCADIA_API extern auto to_json(const glm::vec3& vec) -> nlohmann::json;
        ARCADIA_API extern auto from_json(const nlohmann::json& json) -> glm::vec3;

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

    namespace ivec3
    {
        using serialization_type = arcadia::serialization::ivec3;

        ARCADIA_API extern auto to_flatbuffers(const glm::ivec3& vec) -> serialization_type;
        ARCADIA_API extern auto from_flatbuffers(const serialization_type& flat_vec) -> glm::ivec3;
    }

    namespace uvec3
    {
        using serialization_type = arcadia::serialization::uvec3;

        ARCADIA_API extern auto to_flatbuffers(const glm::uvec3& vec) -> serialization_type;
        ARCADIA_API extern auto from_flatbuffers(const serialization_type& flat_vec) -> glm::uvec3;
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
