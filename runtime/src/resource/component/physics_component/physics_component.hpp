#pragma once

#include"core/base.hpp"
#include"core/nlohmann_json_header.hpp"
#include"resource/component/component.hpp"

namespace arcadia
{
    struct ARCADIA_API physics_component: arcadia::component_base
    {
    public:
        using self_type = physics_component;
    public:
        ARCADIA_COMPONENT_TYPE_STR_GETERS("physics");

        physics_component() = default;
        physics_component(const nlohmann::json& json);
        ~physics_component() = default;
        [[nodiscard]]
        auto to_json() const->nlohmann::json;


    };
}
