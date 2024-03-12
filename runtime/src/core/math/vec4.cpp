#include "pch.hpp"
#include "vec4.hpp"

ARCADIA_API auto arcadia::vec4::to_json(const glm::vec4& vec) -> nlohmann::json
{
    return nlohmann::json{
        {"x",vec.x},
        {"y",vec.y},
        {"z",vec.z},
        {"w",vec.w}
    };
}

ARCADIA_API auto arcadia::vec4::from_json(const nlohmann::json& json) -> glm::vec4
{
    glm::vec4 vec{};
    vec.x = json.at("x");
    vec.y = json.at("y");
    vec.z = json.at("z");
    vec.w = json.at("w");
    return vec;
}
