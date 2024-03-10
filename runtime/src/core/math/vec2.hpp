#pragma once

#include"flatbuffers_generated/math_generated.h"

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace arcadia
{

    namespace vec2
    {

        using serialization_type = arcadia::serialization::vec2;

        extern auto to_flatbuffers(const glm::vec2& vec) -> serialization_type;
        extern auto from_flatbuffers(const serialization_type& flat_vec) -> glm::vec2;

        extern auto to_json(const glm::vec2& vec) -> nlohmann::json;
        extern auto from_json(const nlohmann::json& json) -> glm::vec2;

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


    namespace ivec2
    {
        using serialization_type = arcadia::serialization::ivec2;

        extern auto to_flatbuffers(const glm::ivec2& vec) -> serialization_type;
        extern auto from_flatbuffers(const serialization_type& flat_vec) -> glm::ivec2;

        extern auto to_json(const glm::ivec2& vec) -> nlohmann::json;
        extern auto from_json(const nlohmann::json& json) -> glm::ivec2;
    }

    namespace uvec2
    {
        using serialization_type = arcadia::serialization::uvec2;

        extern auto to_flatbuffers(const glm::uvec2& vec) -> serialization_type;
        extern auto from_flatbuffers(const serialization_type& flat_vec) -> glm::uvec2;
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
