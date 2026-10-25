#include "script_service.hpp"

#include "core/function.hpp"

Arcadia::ScriptService::ScriptService(Engine& engine):
    _Engine(engine)
{
    _EventSubscription = _Engine.events.SubscribeAny(
        Foundation::Order::Script,
        [this](Foundation::Event& e) -> bool
        {
            OnEvent(e);
            return e.Handled();
        }
    );
}

void Arcadia::ScriptService::Update()
{
    OnUpdate();
}

void Arcadia::ScriptService::OnEvent(EventBase& event)
{
    EventDispatcher(event)
        .Dispatch<Events::ExecuteScript>(ACDA_BIND_MEMBER_FN(_OnExecuteScript))
        .IsDispatched();
}

void Arcadia::ScriptService::OnUpdate()
{
}

void Arcadia::ScriptService::_OnExecuteScript(Events::ExecuteScript& e)
{
    _ScriptInterpreter.ExecuteScript(e.ScriptText);
}
