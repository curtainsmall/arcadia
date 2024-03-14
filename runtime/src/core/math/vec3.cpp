#include "pch.hpp"
#include "vec3.hpp"

auto arcadia::vec3::to_json(const glm::vec3& vec) -> nlohmann::json
{
    return nlohmann::json{
        { "x",vec.x },
        { "y",vec.y },
        { "z",vec.z }
    };
}

auto arcadia::vec3::from_json(const nlohmann::json& json) -> glm::vec3
{
    glm::vec3 vec{};
    vec.x = json.at("x");
    vec.y = json.at("y");
    vec.z = json.at("z");
    return vec;
}

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

ARCADIA_API auto arcadia::dvec3::to_json(const glm::dvec3& vec) -> nlohmann::json
{
    return nlohmann::json{
         { "x",vec.x },
         { "y",vec.y },
         { "z",vec.z }
    };
}

ARCADIA_API auto arcadia::dvec3::from_json(const nlohmann::json& json) -> glm::dvec3
{
    glm::dvec3 vec{};
    vec.x = json.at("x");
    vec.y = json.at("y");
    vec.z = json.at("z");
    return vec;
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


