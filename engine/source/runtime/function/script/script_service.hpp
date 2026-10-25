#pragma once

#include <memory>

#include "platform/defines.hpp"
#include "app/engine.hpp"
#include "app/services.hpp"
#include "core/event.hpp"
#include "function/script/script_events.hpp"
#include "function/script/script_interpreter.hpp"

namespace Arcadia
{
    struct ACDA_API ScriptService: public ScriptHost
    {
    public:
        using SelfType = ScriptService;
    public:
        ScriptService(Engine& engine);
        virtual ~ScriptService() override = default;

        void Update() override;

        void OnEvent(EventBase& event);
        void OnUpdate();
    private:
        void _OnExecuteScript(Events::ExecuteScript& e);
    private:
        Engine& _Engine;

        Foundation::Subscription _EventSubscription{};

        ScriptInterpreter _ScriptInterpreter{};
    };
}
