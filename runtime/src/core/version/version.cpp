#include "pch.hpp"

#include "version.hpp"

Version::Version(const nlohmann::json& json):
    major(json.at("major")),
    minor(json.at("minor")),
    patch(json.at("patch"))
{}

auto Version::to_json() const -> nlohmann::json
{
    return nlohmann::json{
        {"major",major},
        {"minor",minor},
        {"patch",patch}
    };
}

Version::operator std::string() const
{
    return std::format("{0}.{1}.{2}", major, minor, patch);
}
