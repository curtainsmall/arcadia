#include "player_service.hpp"

#include "core/function.hpp"
#include "core/log.hpp"
#include "resource/scene_service.hpp"

Arcadia::PlayerService::PlayerService(Engine& engine):
    _Engine(engine),
    _PlayerController(engine)
{
    _EventSubscription = _Engine.events.SubscribeAny(
        Foundation::Order::Player,
        [this](Foundation::Event& e) -> bool
        {
            OnEvent(e);
            return e.Handled();
        }
    );
}

void Arcadia::PlayerService::Update()
{
    OnUpdate();
}

void Arcadia::PlayerService::OnEvent(EventBase& event)
{
    EventDispatcher(event)
        .Dispatch<Events::PlayerControllerSetActive>(ACDA_BIND_MEMBER_FN(_OnPlayerControllerSetActive))
        .Dispatch<Events::PlayerControllerSetEntity>(ACDA_BIND_MEMBER_FN(_OnPlayerControllerSetEntity))
        .HaltDispatchIf(!IsPlayerControllerActive() || GetActiveEntityId().IsNull())
        .Dispatch<Events::InputKey>(ACDA_BIND_MEMBER_FN(_OnInputKey))
        .Dispatch<Events::InputCursorMove>(ACDA_BIND_MEMBER_FN(_OnInputCursorMove))
        .Dispatch<Events::InputScroll>(ACDA_BIND_MEMBER_FN(_OnInputScroll))
        .Dispatch<Events::InputMouseButton>(ACDA_BIND_MEMBER_FN(_OnInputMouseButton))
        .IsDispatched();
}

void Arcadia::PlayerService::OnUpdate()
{
}

auto Arcadia::PlayerService::IsPlayerControllerActive() const -> bool
{
    return _PlayerController.IsActive();
}

auto Arcadia::PlayerService::GetActiveEntityId() const -> EntityId
{
    return _PlayerController.GetActiveEntityId();
}

void Arcadia::PlayerService::_OnPlayerControllerSetActive(Events::PlayerControllerSetActive& e)
{
    ACDA_LOG_DEBUG(std::format("Player Controller set: {}", e.Active));
    _PlayerController.SetActive(e.Active);
}

void Arcadia::PlayerService::_OnPlayerControllerSetEntity(Events::PlayerControllerSetEntity& e)
{
    SceneService* scene_service = _Engine.scenes;
    if(!scene_service->ActiveScene_ContainsEntity(e.EntityId))
    {
        return;
    }

    _PlayerController.SetActiveEntityId(e.EntityId);
}

void Arcadia::PlayerService::_OnInputKey(Events::InputKey& e)
{
    _PlayerController.ProcessInputKey(e.KeyCode, e.Action, e.Modifier);
}

void Arcadia::PlayerService::_OnInputCursorMove(Events::InputCursorMove& e)
{
}

void Arcadia::PlayerService::_OnInputScroll(Events::InputScroll& e)
{
}

void Arcadia::PlayerService::_OnInputMouseButton(Events::InputMouseButton& e)
{
    _PlayerController.ProcessInputMouseButton(e.MouseButton, e.Action, e.Modifier);
}
