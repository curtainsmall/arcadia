#pragma once

#include "core/event.hpp"
#include "function/physics/physics_simulator.hpp"

namespace Arcadia::Events
{
    class PhysicsSimulatorReset: public EventBase
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

    class PhysicsSimulatorSetEntity: public EventBase
    {
    public:
        enum class ActionType
        {
            Add,
            Remove,
            Update
        };
    public:
        PhysicsSimulatorSetEntity(EntityId entitiy_id, ActionType active_type):
            EntityId(entitiy_id),
            ActionType(active_type)
        {
        }
    public:
        EntityId EntityId;
        ActionType ActionType;
    };
}