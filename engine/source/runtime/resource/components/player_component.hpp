#pragma once

#include "core/nlohmann_json.hpp"
#include "platform/defines.hpp"
#include "resource/components/component.hpp"

namespace Arcadia
{
    struct ACDA_API PlayerComponent: public Noncopyable
    {
    public:
        using SelfType = PlayerComponent;
    public:
        ACDA_COMPONENT_TYPE_STR_GETTERS("player");

        PlayerComponent() = default;
        PlayerComponent(const nlohmann::json& json);
        virtual ~PlayerComponent() = default;
        [[nodiscard]]
        auto ToJson() const -> nlohmann::json;
    };
}