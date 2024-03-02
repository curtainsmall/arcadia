#include "pch.hpp"
#include "version.hpp"

auto arcadia::version::to_flatbuffers(const self_type& version) -> serialization_type
{
    return {
        version.major,
        version.minor,
        version.patch
    };
}

auto arcadia::version::from_flatbuffers(const serialization_type& flat_version) -> self_type
{
    return {
        flat_version.major(),
        flat_version.minor(),
        flat_version.patch()
    };
}

arcadia::version::version(const nlohmann::json& json):
    major(json.at("major")),
    minor(json.at("minor")),
    patch(json.at("patch"))
{}

auto arcadia::version::to_json() const -> nlohmann::json
{
    return nlohmann::json{
        {"major",major},
        {"minor",minor},
        {"patch",patch}
    };
}

arcadia::version::operator std::string() const
{
    return std::format("{0}.{1}.{2}", major, minor, patch);
}
