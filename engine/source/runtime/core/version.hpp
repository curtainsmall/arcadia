#pragma once

#include <string>

#include "core/nlohmann_json.hpp"
#include "platform/defines.hpp"

namespace Arcadia
{
    struct ACDA_API Version
    {
    public:
        using NumType = int;
        using SelfType = Version;
    public:
        Version() = default;
        Version(NumType major, NumType minor, NumType patch):
            Major(major),
            Minor(minor),
            Patch(patch)
        {}
        Version(const nlohmann::json& json);
        auto ToJson() const->nlohmann::json;

        auto operator<=>(const SelfType& rhs) const = default;

        operator std::string() const;

    public:
        NumType Major{ 0 };
        NumType Minor{ 0 };
        NumType Patch{ 0 };
    };
}

namespace std
{
    template<>
    struct ACDA_API formatter<Arcadia::Version>: public std::formatter<std::string>
    {
    public:
        auto format(const Arcadia::Version& version, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{}.{}.{}", version.Major, version.Minor, version.Patch),
                ctx
            );
        }
    };
}