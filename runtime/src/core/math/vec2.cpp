#include "pch.hpp"
#include "vec2.hpp"

auto arcadia::vec2::to_flatbuffers(const glm::vec2& vec) -> serialization_type
{
    return {
        vec.x,
        vec.y
    };
}

auto arcadia::vec2::from_flatbuffers(const serialization_type& flat_vec) -> glm::vec2
{
    return {
        flat_vec.x(),
        flat_vec.y()
    };
}

auto arcadia::ivec2::to_flatbuffers(const glm::ivec2& vec) -> serialization_type
{
    return {
        vec.x,
        vec.y
    };
}

auto arcadia::ivec2::from_flatbuffers(const serialization_type& flat_vec) -> glm::ivec2
{
    return {
            flat_vec.x(),
            flat_vec.y()
    };
}

auto arcadia::uvec2::to_flatbuffers(const glm::uvec2& vec) -> serialization_type
{
    return {
        vec.x,
        vec.y
    };
}

auto arcadia::uvec2::from_flatbuffers(const serialization_type& flat_vec) -> glm::uvec2
{
    return {
        flat_vec.x(),
        flat_vec.y()
    };
}
