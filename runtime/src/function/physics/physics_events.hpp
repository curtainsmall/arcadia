#pragma once

#include "core/event.hpp"
#include "function/physics/physics_simulator.hpp"

namespace Arcadia::Events
{
    class PhysicsSimulatorBuilt: public EventBase
    {
    public:
        PhysicsSimulatorBuilt(const std::shared_ptr<PhysicsSimulator>& physcis_simulator_sptr):
            spPhysicsSimulator(physcis_simulator_sptr)
        {
        }
    public:
        const std::shared_ptr<PhysicsSimulator> spPhysicsSimulator;
    };

    class PhysicsSimulatorUnbuilt: public EventBase
    {
    };

    class PhysicsSimulatirSetActive: public EventBase
    {
    public:
        PhysicsSimulatirSetActive(bool active):
            Active(active)
        {
        }

    public:
        const bool Active;
    };

    class PhysicsSimulatorReset: public EventBase
    {
    };

    class PhysicsSimulatorSetScene: public EventBase
    {
    public:
        PhysicsSimulatorSetScene(const std::shared_ptr<Scene>& scene_sptr):
            spScene(scene_sptr)
        {
        }
    public:
        std::shared_ptr<Scene> spScene;
    };

    enum class PhysicsSimulatorSetEntity_ActionType
    {
        Add,
        Remove,
        Update,
    };
    class PhysicsSimulatorSetEntity: public EventBase
    {
    public:
        PhysicsSimulatorSetEntity(EntityId entitiy_id, PhysicsSimulatorSetEntity_ActionType active_type):
            EntityId(entitiy_id),
            ActionType(active_type)
        {
        }
    public:
        EntityId EntityId;
        PhysicsSimulatorSetEntity_ActionType ActionType;
    };
}