#pragma once

#include <memory>

#include "platform/defines.hpp"
#include "core/layer.hpp"
#include "core/event.hpp"
#include "function/script/script_events.hpp"
#include "function/script/script_interpreter.hpp"

namespace Arcadia
{
    struct ACDA_API ScriptLayer: public LayerInterface
    {
    public:
        using SelfType = ScriptLayer;
    public:
        ScriptLayer();
        virtual ~ScriptLayer() override = default;

        virtual void OnEvent(EventBase& event) override;
        virtual void OnUpdate() override;
    private:
        void _OnExecuteScript(Events::ExecuteScript& e);
    private:
        ScriptInterpreter _ScriptInterpreter{};
    };
}