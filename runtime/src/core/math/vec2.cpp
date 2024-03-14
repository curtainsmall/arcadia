#include "pch.hpp"
#include "vec2.hpp"

auto arcadia::vec2::to_json(const glm::vec2& vec) -> nlohmann::json
{
    return nlohmann::json{
     { "x",vec.x },
     { "y",vec.y }
    };
}

auto arcadia::vec2::from_json(const nlohmann::json& json) -> glm::vec2
{
    glm::vec2 vec{};
    vec.x = json.at("x");
    vec.y = json.at("y");
    return vec;
}

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

auto arcadia::dvec2::to_json(const glm::dvec2& vec) -> nlohmann::json
{
    return nlohmann::json{
        { "x",vec.x },
        { "y",vec.y }
    };
}

auto arcadia::dvec2::from_json(const nlohmann::json& json) -> glm::dvec2
{
    glm::dvec2 vec{};
    vec.x = json.at("x");
    vec.y = json.at("y");
    return vec;
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

auto arcadia::ivec2::to_json(const glm::ivec2& vec) -> nlohmann::json
{
    return nlohmann::json{
        {"x",vec.x},
        {"y",vec.y}
    };
}

auto arcadia::ivec2::from_json(const nlohmann::json& json) -> glm::ivec2
{
    return glm::ivec2{
        json.at("x"),
        json.at("y")
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


