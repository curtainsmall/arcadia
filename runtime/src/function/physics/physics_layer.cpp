#include "pch.hpp"
#include "physics_layer.hpp"

#include "core/function.hpp"
#include "function/physics/physics_events.hpp"

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
        .Dispatch<Events::PhysicsSimulatorSetScene>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorSetScene))
        .Dispatch<Events::PhysicsSimulatorSetEntity>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorSetEntity))
        .IsDispatched();
}

void Arcadia::PhysicsLayer::OnUpdate()
{
    _spPhysicsSimulator->Update();
}

auto Arcadia::PhysicsLayer::IsPhysicsSimulatorActive() const -> bool
{
    return _spPhysicsSimulator->IsActive();
}

auto Arcadia::PhysicsLayer::GetPhysicsBodyCount() const -> std::size_t
{
    return _spPhysicsSimulator->GetBodyCount();
}

auto Arcadia::PhysicsLayer::GetTempAllocatorSize() const -> std::size_t
{
    return _spPhysicsSimulator->GetJphTempAllocatorSize();
}

auto Arcadia::PhysicsLayer::GetUpdatesPerSecondCount() const -> std::int32_t
{
    return _spPhysicsSimulator->GetJphPhysicsSystemUpdatesPerSecond();
}

void Arcadia::PhysicsLayer::_OnPhysicsSimulatorSetActive(Events::PhysicsSimulatirSetActive& e)
{
    _spPhysicsSimulator->SetActive(e.Active);
}

void Arcadia::PhysicsLayer::_OnPhysicsSimulatorReset(Events::PhysicsSimulatorReset& e)
{
    _spPhysicsSimulator->Reset();
}

void Arcadia::PhysicsLayer::_OnPhysicsSimulatorSetScene(Events::PhysicsSimulatorSetScene& e)
{
    _spPhysicsSimulator->SetScene(e.spScene);
}

void Arcadia::PhysicsLayer::_OnPhysicsSimulatorSetEntity(Events::PhysicsSimulatorSetEntity& e)
{
    switch(e.ActionType)
    {
        case Events::PhysicsSimulatorSetEntity::ActionType::Add:
        {
            _spPhysicsSimulator->AddEntity(e.EntityId);
            break;
        }
        case Events::PhysicsSimulatorSetEntity::ActionType::Remove:
        {
            _spPhysicsSimulator->RemoveEntity(e.EntityId);
            break;
        }
        case Events::PhysicsSimulatorSetEntity::ActionType::Update:
        {
            _spPhysicsSimulator->UpdateEntity(e.EntityId);
            break;
        }
        default:
        {
            ACDA_UNREACHABLE("Invalid action type");
        }
    }
}