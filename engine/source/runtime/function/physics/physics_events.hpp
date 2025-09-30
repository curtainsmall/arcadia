#pragma once

#include "core/event.hpp"
#include "function/physics/physics_simulator.hpp"

namespace Arcadia::Events
{
    struct ACDA_API PhysicsSimulatorReset: public EventBase
    {
    };

    struct ACDA_API PhysicsSimulatorSetActive: public EventBase
    {
    public:
        PhysicsSimulatorSetActive(bool active):
            Active(active)
        {
        }

    public:
        const bool Active;
    };

    struct ACDA_API PhysicsSimulatorSetEntity: public EventBase
    {
    public:
        enum struct ACDA_API ActionType
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