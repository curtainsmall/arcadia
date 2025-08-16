#include "player_controller.hpp"

#include "resource/components.hpp"
#include "resource/scene_layer.hpp"
#include "core/math.hpp"

auto Arcadia::PlayerController::IsActive() const -> bool
{
    return _Active;
}

void Arcadia::PlayerController::SetActive(bool active)
{
    _Active = active;
}

auto Arcadia::PlayerController::GetActiveEntityId() const -> EntityId
{
    return _ActiveEntityId;
}

void Arcadia::PlayerController::SetActiveEntityId(EntityId entity_id)
{
    _ActiveEntityId = entity_id;
}

void Arcadia::PlayerController::ProcessInputKey(GlfwInputKey key, GlfwInputAction action, GlfwInputModifier mod)
{
    std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();
    auto [transform_comp, physics_comp] = scene_layer_sptr->ActiveScene_GetComponent<TransformComponent, PhysicsComponent>(_ActiveEntityId);

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
            physics_comp.SetLinearVelocity(
                glm::normalize(
                    glm::cross(
                        CameraComponent::GetUpAxis(),
                        transform_comp.GetDirection()
                    )
                )
                * speed
            );
            break;
        }
        case GlfwInputKey::D:
        {
            physics_comp.SetLinearVelocity(
                -glm::normalize(
                    glm::cross(
                        CameraComponent::GetUpAxis(),
                        transform_comp.GetDirection()
                    )
                )
                * speed
            );
            break;
        }
    }
}
void Arcadia::PlayerController::ProcessInputMouseButton(GlfwInputMouseButton mouse_button, GlfwInputAction action, GlfwInputModifier mod)
{
}