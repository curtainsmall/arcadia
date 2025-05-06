#include "pch.hpp"
#include "physics_layer.hpp"

#include "core/function.hpp"
#include "function/physics/physics_events.hpp"

Arcadia::PhysicsLayer::PhysicsLayer():
    LayerInterface("physics")
{
    _spPhysicsSimulator = std::make_shared<PhysicsSimulator>();
    EventQueue::Instance().Signal<Events::PhysicsSimulatorBuilt>(_spPhysicsSimulator);
}

Arcadia::PhysicsLayer::~PhysicsLayer()
{
    EventQueue::Instance().Signal<Events::PhysicsSimulatorUnbuilt>();
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
        case Events::PhysicsSimulatorSetEntity_ActionType::Add:
        {
            _spPhysicsSimulator->AddEntity(e.EntityId);
            break;
        }
        case Events::PhysicsSimulatorSetEntity_ActionType::Remove:
        {
            _spPhysicsSimulator->RemoveEntity(e.EntityId);
            break;
        }
        case Events::PhysicsSimulatorSetEntity_ActionType::Update:
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