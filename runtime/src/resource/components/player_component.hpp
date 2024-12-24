#pragma once

#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"platform/api_def.hpp"
#include"resource/components/component_interface.hpp"

namespace Arcadia
{
    class PlayerComponent:
        public ComponentInterface,
        public MementoOriginatorInterface
    {
    public:
        using SelfType = PlayerComponent;
    public:
        ACDA_COMPONENT_TYPE_STR_GETTERS("player");

        PlayerComponent() = default;
        PlayerComponent(const nlohmann::json& json);
        ~PlayerComponent() = default;
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;

    protected:
        [[nodiscard]]
        virtual auto OnSnapshot() const->std::shared_ptr<MementoDataBase> override;
        virtual void OnRestore(const std::shared_ptr<MementoDataBase>& memento_data) override;
    };
}