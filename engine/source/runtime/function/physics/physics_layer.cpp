#include "physics_layer.hpp"

#include "core/function.hpp"
#include "function/physics/physics_events.hpp"
#include "resource/scene_layer.hpp"

Arcadia::PhysicsLayer::PhysicsLayer():
    LayerInterface("physics")
{
}

void Arcadia::PhysicsLayer::OnEvent(EventBase& event)
{
    EventDispatcher(event)
        .Dispatch<Events::PhysicsSimulatorSetActive>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorSetActive))
        .Dispatch<Events::PhysicsSimulatorReset>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorReset))
        .Dispatch<Events::PhysicsSimulatorSetEntity>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorSetEntity))
        .IsDispatched();
}

void Arcadia::PhysicsLayer::OnUpdate()
{
    if(!_PhysicsSimulator.IsActive())
    {
        return;
    }

    _PhysicsSimulator.Update();
    _PhysicsSimulator.ApplyToEntites();
}

auto Arcadia::PhysicsLayer::IsPhysicsSimulatorActive() const -> bool
{
    return _PhysicsSimulator.IsActive();
}

auto Arcadia::PhysicsLayer::GetPhysicsBodyCount() const -> std::size_t
{
    return _PhysicsSimulator.GetBodyCount();
}

auto Arcadia::PhysicsLayer::GetTempAllocatorSize() const -> std::size_t
{
    return _PhysicsSimulator.GetJphTempAllocatorSize();
}

auto Arcadia::PhysicsLayer::GetUpdatesPerSecondCount() const -> std::int32_t
{
    return _PhysicsSimulator.GetJphPhysicsSystemUpdatesPerSecond();
}

void Arcadia::PhysicsLayer::_OnPhysicsSimulatorSetActive(Events::PhysicsSimulatorSetActive& e)
{
    _PhysicsSimulator.SetActive(e.Active);
}

void Arcadia::PhysicsLayer::_OnPhysicsSimulatorReset(Events::PhysicsSimulatorReset& e)
{
    _PhysicsSimulator.Reset();
}

void Arcadia::PhysicsLayer::_OnPhysicsSimulatorSetEntity(Events::PhysicsSimulatorSetEntity& e)
{
    std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();
    if(!scene_layer_sptr->HasActiveScene() || !scene_layer_sptr->ActiveScene_ContainsEntity(e.EntityId))
    {
        return;
    }

    switch(e.ActionType)
    {
        case Events::PhysicsSimulatorSetEntity::ActionType::Build:
        {
            _PhysicsSimulator.BuildEntity(e.EntityId);
            break;
        }
        case Events::PhysicsSimulatorSetEntity::ActionType::Remove:
        {
            _PhysicsSimulator.RemoveEntity(e.EntityId);
            break;
        }
        default:
        {
            ACDA_UNREACHABLE("Invalid action type");
        }
    }
}