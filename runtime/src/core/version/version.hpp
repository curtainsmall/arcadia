#pragma once

#include <string>

#include"core/base.hpp"
#include"core/nlohmann_json_header.hpp"

struct Version
{
public:
    using num_type = int;
    using self_type = Version;
public:
    Version() = default;
    Version(num_type major, num_type minor, num_type patch):
        Major(major),
        Minor(minor),
        Patch(patch)
    {}
    Version(const nlohmann::json& json);
    auto ToJson() const->nlohmann::json;

    auto operator<=>(const self_type& rhs) const = default;

    operator std::string() const;

public:
    num_type Major{ 0 };
    num_type Minor{ 0 };
    num_type Patch{ 0 };
};

namespace std
{
    template<>
    struct std::formatter<Version>: std::formatter<std::string>
    {
        auto format(const Version& version, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{}.{}.{}", version.Major, version.Minor, version.Patch),
                ctx
            );
        }
    };
}
