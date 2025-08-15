#include "player_layer.hpp"

#include "resource/scene_layer.hpp"
#include "resource/components.hpp"
#include "core/function.hpp"
#include "function/window/window_events.hpp"
#include "core/match.hpp"

void Arcadia::PlayerLayer::OnEvent(EventBase& event)
{
    EventDispatcher(event)
        .Dispatch<Events::PlayerControllerSetActive>(ACDA_BIND_MEMBER_FN(_OnPlayerControllerSetActive))
        .Dispatch<Events::PlayerControllerSetEntity>(ACDA_BIND_MEMBER_FN(_OnPlayerControllerSetEntity))
        .HaltDispatchIf(!IsPlayerControllerActive())
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
    _ProcessInputKey(e.KeyCode, e.Action, e.Modifier);
}

void Arcadia::PlayerLayer::_OnInputCursorMove(Events::InputCursorMove& e)
{
}

void Arcadia::PlayerLayer::_OnInputScroll(Events::InputScroll& e)
{
}

void Arcadia::PlayerLayer::_OnInputMouseButton(Events::InputMouseButton& e)
{
}

void Arcadia::PlayerLayer::_ProcessInputKey(GlfwInputKey key, GlfwInputAction action, GlfwInputModifier mod)
{
    std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();
    auto [transform_comp, physics_comp] = scene_layer_sptr->ActiveScene_GetComponent<TransformComponent, PhysicsComponent>(_PlayerController.GetActiveEntityId());

    const float speed = 5.0f;
    switch(key)
    {
        case GlfwInputKey::W:
        {
            physics_comp.SetLinearVelocity(glm::normalize(transform_comp.GetDirection()) * speed);
            break;
        }
        case GlfwInputKey::S:
        {
            physics_comp.SetLinearVelocity(-glm::normalize(transform_comp.GetDirection()) * speed);
            break;
        }
        case GlfwInputKey::A:
        {
            physics_comp.SetLinearVelocity(glm::normalize(glm::cross(CameraComponent::GetUpAxis(), transform_comp.GetDirection()) * speed));
            break;
        }
        case GlfwInputKey::D:
        {
            physics_comp.SetLinearVelocity(-glm::normalize(glm::cross(CameraComponent::GetUpAxis(), transform_comp.GetDirection()) * speed));
            break;
        }
    }
}

void Arcadia::PlayerLayer::_ProcessInputMouseButton(GlfwInputMouseButton mouse_button, GlfwInputAction action, GlfwInputModifier mod)
{
}