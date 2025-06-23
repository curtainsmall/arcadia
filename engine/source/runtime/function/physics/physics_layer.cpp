
#include "physics_layer.hpp"

#include "core/function.hpp"
#include "function/physics/physics_events.hpp"
#include "resource/scene_layer.hpp"

Arcadia::PhysicsLayer::PhysicsLayer():
    LayerInterface("physics")
{
    _spPhysicsSimulator = std::make_shared<PhysicsSimulator>();
}

void Arcadia::PhysicsLayer::OnEvent(EventBase& event)
{
    EventDispatcher(event)
        .Dispatch<Events::PhysicsSimulatirSetActive>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorSetActive))
        .Dispatch<Events::PhysicsSimulatorReset>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorReset))
        .Dispatch<Events::PhysicsSimulatorSetEntity>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorSetEntity))
        .IsDispatched();
}

void Arcadia::PhysicsLayer::OnUpdate()
{
    ACDA_ASSERT(_spPhysicsSimulator);

    if(!_spPhysicsSimulator->IsActive())
    {
        return;
    }

    _spPhysicsSimulator->Update();
    _spPhysicsSimulator->ApplyToEntity();
}

auto Arcadia::PhysicsLayer::IsPhysicsSimulatorActive() const -> bool
{
    ACDA_ASSERT(_spPhysicsSimulator);
    return _spPhysicsSimulator->IsActive();
}

auto Arcadia::PhysicsLayer::GetPhysicsBodyCount() const -> std::size_t
{
    ACDA_ASSERT(_spPhysicsSimulator);
    return _spPhysicsSimulator->GetBodyCount();
}

auto Arcadia::PhysicsLayer::GetTempAllocatorSize() const -> std::size_t
{
    ACDA_ASSERT(_spPhysicsSimulator);
    return _spPhysicsSimulator->GetJphTempAllocatorSize();
}

auto Arcadia::PhysicsLayer::GetUpdatesPerSecondCount() const -> std::int32_t
{
    ACDA_ASSERT(_spPhysicsSimulator);
    return _spPhysicsSimulator->GetJphPhysicsSystemUpdatesPerSecond();
}

void Arcadia::PhysicsLayer::_OnPhysicsSimulatorSetActive(Events::PhysicsSimulatirSetActive& e)
{
    ACDA_ASSERT(_spPhysicsSimulator);
    _spPhysicsSimulator->SetActive(e.Active);
}

void Arcadia::PhysicsLayer::_OnPhysicsSimulatorReset(Events::PhysicsSimulatorReset& e)
{
    ACDA_ASSERT(_spPhysicsSimulator);
    _spPhysicsSimulator->Reset();
}

void Arcadia::PhysicsLayer::_OnPhysicsSimulatorSetEntity(Events::PhysicsSimulatorSetEntity& e)
{
    ACDA_ASSERT(_spPhysicsSimulator);

    std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();
    if(!scene_layer_sptr->HasActiveScene() || !scene_layer_sptr->ActiveScene_ContainsEntity(e.EntityId))
    {
        return;
    }

    switch(e.ActionType)
    {
        case Events::PhysicsSimulatorSetEntity::ActionType::Build:
        {
            _spPhysicsSimulator->BuildEntity(e.EntityId);
            break;
        }
        case Events::PhysicsSimulatorSetEntity::ActionType::Remove:
        {
            _spPhysicsSimulator->RemoveEntity(e.EntityId);
            break;
        }
        default:
        {
            ACDA_UNREACHABLE("Invalid action type");
        }
    }
}