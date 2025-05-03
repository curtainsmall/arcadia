#include "pch.hpp"
#include "physics_layer.hpp"

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
{}

void Arcadia::PhysicsLayer::OnUpdate()
{}
