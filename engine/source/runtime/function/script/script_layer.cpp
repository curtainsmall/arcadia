#include "script_layer.hpp"

#include "core/function.hpp"

Arcadia::ScriptLayer::ScriptLayer():
    LayerInterface("script")
{
}

void Arcadia::ScriptLayer::OnEvent(EventBase& event)
{
    EventDispatcher(event)
        .Dispatch<Events::ExecuteScript>(ACDA_BIND_MEMBER_FN(_OnExecuteScript))
        .IsDispatched();
}

void Arcadia::ScriptLayer::OnUpdate()
{
}

void Arcadia::ScriptLayer::_OnExecuteScript(Events::ExecuteScript& e)
{
    _ScriptInterpreter.ExecuteScript(e.ScriptText);
}