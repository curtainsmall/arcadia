#include "pch.hpp"
#include "vec3.hpp"

auto arcadia::vec3::to_flatbuffers(const glm::vec3& vec) -> serialization_type
{
    return {
        vec.x,
        vec.y,
        vec.z
    };
}

auto arcadia::vec3::from_flatbuffers(const serialization_type& flat_vec) -> glm::vec3
{
    return {
        flat_vec.x(),
        flat_vec.y(),
        flat_vec.z()
    };
}

auto arcadia::ivec3::to_flatbuffers(const glm::ivec3& vec) -> serialization_type
{
    return {
        vec.x,
        vec.y,
        vec.z
    };
}

auto arcadia::ivec3::from_flatbuffers(const serialization_type& flat_vec) -> glm::ivec3
{
    return {
        flat_vec.x(),
        flat_vec.y(),
        flat_vec.z()
    };
}

auto arcadia::uvec3::to_flatbuffers(const glm::uvec3& vec) -> serialization_type
{
    return {
        vec.x,
        vec.y,
        vec.z
    };
}

auto arcadia::uvec3::from_flatbuffers(const serialization_type& flat_vec) -> glm::uvec3
{
    return {
        flat_vec.x(),
        flat_vec.y(),
        flat_vec.z()
    };
}
