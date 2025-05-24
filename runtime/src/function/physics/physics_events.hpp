#pragma once

#include "core/event.hpp"
#include "function/physics/physics_simulator.hpp"

namespace Arcadia::Events
{
    struct PhysicsSimulatorReset: public EventBase
    {
    };

    struct PhysicsSimulatirSetActive: public EventBase
    {
    public:
        PhysicsSimulatirSetActive(bool active):
            Active(active)
        {
        }

    public:
        const bool Active;
    };

    struct PhysicsSimulatorSetEntity: public EventBase
    {
    public:
        enum struct ActionType
        {
            Build,
            Remove,
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