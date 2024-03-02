#pragma once

#include <string>

#include"flatbuffers_generated/meta_generated.h"

#include"core/base.hpp"
#include"core/nlohmann_json_header.hpp"

namespace arcadia
{
    struct ARCADIA_API version
    {
    public:
        using num_type = int;

        using self_type = arcadia::version;
        using serialization_type = arcadia::serialization::version;
    public:
        static auto to_flatbuffers(const self_type& version) -> serialization_type;
        static auto from_flatbuffers(const serialization_type& flat_version) -> self_type;

        version() = default;
        inline version(num_type major, num_type minor, num_type patch):
            major(major),
            minor(minor),
            patch(patch)
        {}
        version(const nlohmann::json& json);
        auto to_json() const->nlohmann::json;

        auto operator<=>(const self_type& rhs) const = default;

        operator std::string() const;

    public:
        num_type major{ 0 };
        num_type minor{ 0 };
        num_type patch{ 0 };
    };
}
