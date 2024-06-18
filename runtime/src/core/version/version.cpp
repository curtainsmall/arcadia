#include "pch.hpp"

#include "version.hpp"

Version::Version(const nlohmann::json& json):
    Major(json.at("major")),
    Minor(json.at("minor")),
    Patch(json.at("patch"))
{}

auto Version::ToJson() const -> nlohmann::json
{
    return nlohmann::json{
        {"major",Major},
        {"minor",Minor},
        {"patch",Patch}
    };
}

Version::operator std::string() const
{
    return std::format("{0}.{1}.{2}", Major, Minor, Patch);
}
