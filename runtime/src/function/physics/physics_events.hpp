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
        {}
    public:
        const std::shared_ptr<PhysicsSimulator> spPhysicsSimulator;
    };

    class PhysicsSimulatorUnbuilt: public EventBase
    {};
}
