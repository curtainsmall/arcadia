#pragma once

#include <memory>

#include "core/layer.hpp"
#include "core/event.hpp"
#include "function/physics/physics_simulator.hpp"

namespace Arcadia
{
    class PhysicsLayer: public LayerInterface
    {
    public:
        using SelfType = PhysicsLayer;
    public:
        PhysicsLayer();
        virtual ~PhysicsLayer();

        virtual void OnEvent(EventBase& event);
        virtual void OnUpdate();

    private:
        std::shared_ptr<PhysicsSimulator> _spPhysicsSimulator{};
    };
}
