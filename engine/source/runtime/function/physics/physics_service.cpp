#include "physics_service.hpp"

#include "core/function.hpp"
#include "function/physics/physics_events.hpp"
#include "resource/scene_service.hpp"

Arcadia::PhysicsService::PhysicsService(Engine& engine):
    _Engine(engine),
    _PhysicsSimulator(engine)
{
    _EventSubscription = _Engine.events.SubscribeAny(
        Foundation::Order::Physics,
        [this](Foundation::Event& e) -> bool
        {
            OnEvent(e);
            return e.Handled();
        }
    );
}

void Arcadia::PhysicsService::Step(std::chrono::nanoseconds delta_time)
{
    (void) delta_time;

    if(!_PhysicsSimulator.IsActive())
    {
        return;
    }

    _PhysicsSimulator.Update();
}

void Arcadia::PhysicsService::ApplyToWorld()
{
    if(!_PhysicsSimulator.IsActive())
    {
        return;
    }

    _PhysicsSimulator.ApplyToEntites();
}

void Arcadia::PhysicsService::OnEvent(EventBase& event)
{
    EventDispatcher(event)
        .Dispatch<Events::PhysicsSimulatorSetActive>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorSetActive))
        .Dispatch<Events::PhysicsSimulatorReset>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorReset))
        .Dispatch<Events::PhysicsSimulatorSetEntity>(ACDA_BIND_MEMBER_FN(_OnPhysicsSimulatorSetEntity))
        .IsDispatched();
}

auto Arcadia::PhysicsService::IsPhysicsSimulatorActive() const -> bool
{
    return _PhysicsSimulator.IsActive();
}

auto Arcadia::PhysicsService::GetPhysicsBodyCount() const -> std::size_t
{
    return _PhysicsSimulator.GetBodyCount();
}

auto Arcadia::PhysicsService::GetTempAllocatorSize() const -> std::size_t
{
    return _PhysicsSimulator.GetJphTempAllocatorSize();
}

auto Arcadia::PhysicsService::GetUpdatesPerSecondCount() const -> std::int32_t
{
    return _PhysicsSimulator.GetJphPhysicsSystemUpdatesPerSecond();
}

void Arcadia::PhysicsService::_OnPhysicsSimulatorSetActive(Events::PhysicsSimulatorSetActive& e)
{
    _PhysicsSimulator.SetActive(e.Active);
}

void Arcadia::PhysicsService::_OnPhysicsSimulatorReset(Events::PhysicsSimulatorReset& e)
{
    _PhysicsSimulator.Reset();
}

void Arcadia::PhysicsService::_OnPhysicsSimulatorSetEntity(Events::PhysicsSimulatorSetEntity& e)
{
    SceneService* scene_service = _Engine.scenes;
    if(!scene_service->HasActiveScene() || !scene_service->ActiveScene_ContainsEntity(e.EntityId))
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
