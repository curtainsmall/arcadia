#pragma once

#include"flatbuffers_generated/math_generated.h"

#include"core/base.hpp"
#include"core/math/glm.hpp"

namespace arcadia
{
    namespace vec2
    {
        using serialization_type = arcadia::serialization::vec2;

        extern auto to_flatbuffers(const glm::vec2& vec) -> serialization_type;
        extern auto from_flatbuffers(const serialization_type& flat_vec) -> glm::vec2;

        ARCADIA_API constexpr auto create_zero() -> glm::vec2
        {
            return glm::vec2{};
        }
        ARCADIA_API constexpr auto create_pos_unit_x() -> glm::vec2
        {
            return glm::vec2{ 1.f,.0f };
        }
        ARCADIA_API constexpr auto create_pos_unit_y() -> glm::vec2
        {
            return glm::vec2{ .0f,1.f };
        }
        ARCADIA_API constexpr auto create_neg_unit_x() -> glm::vec2
        {
            return -create_pos_unit_x();
        }
        ARCADIA_API constexpr auto create_neg_unit_y() -> glm::vec2
        {
            return -create_pos_unit_y();
        }
    }

    namespace ivec2
    {
        using serialization_type = arcadia::serialization::ivec2;

        extern auto to_flatbuffers(const glm::ivec2& vec) -> serialization_type;
        extern auto from_flatbuffers(const serialization_type& flat_vec) -> glm::ivec2;
    }

    namespace uvec2
    {
        using serialization_type = arcadia::serialization::uvec2;

        extern auto to_flatbuffers(const glm::uvec2& vec) -> serialization_type;
        extern auto from_flatbuffers(const serialization_type& flat_vec) -> glm::uvec2;
    }
}
