#include "player_layer.hpp"

#include "core/function.hpp"
#include "core/log.hpp"
#include "resource/scene_layer.hpp"

void Arcadia::PlayerLayer::OnEvent(EventBase& event)
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

void Arcadia::PlayerLayer::OnUpdate()
{
}

auto Arcadia::PlayerLayer::IsPlayerControllerActive() const -> bool
{
    return _PlayerController.IsActive();
}

auto Arcadia::PlayerLayer::GetActiveEntityId() const -> EntityId
{
    return _PlayerController.GetActiveEntityId();
}

void Arcadia::PlayerLayer::_OnPlayerControllerSetActive(Events::PlayerControllerSetActive& e)
{
    ACDA_LOG_DEBUG(std::format("Player Controller set: {}", e.Active));
    _PlayerController.SetActive(e.Active);
}

void Arcadia::PlayerLayer::_OnPlayerControllerSetEntity(Events::PlayerControllerSetEntity& e)
{
    std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();
    if(!scene_layer_sptr->ActiveScene_ContainsEntity(e.EntityId))
    {
        return;
    }

    _PlayerController.SetActiveEntityId(e.EntityId);
}

void Arcadia::PlayerLayer::_OnInputKey(Events::InputKey& e)
{
    _PlayerController.ProcessInputKey(e.KeyCode, e.Action, e.Modifier);
}

void Arcadia::PlayerLayer::_OnInputCursorMove(Events::InputCursorMove& e)
{
}

void Arcadia::PlayerLayer::_OnInputScroll(Events::InputScroll& e)
{
}

void Arcadia::PlayerLayer::_OnInputMouseButton(Events::InputMouseButton& e)
{
    _PlayerController.ProcessInputMouseButton(e.MouseButton, e.Action, e.Modifier);
}