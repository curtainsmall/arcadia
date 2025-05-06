#pragma once

#include <memory>

#include "core/event.hpp"
#include "core/layer.hpp"
#include "function/physics/physics_events.hpp"
#include "function/physics/physics_simulator.hpp"

namespace Arcadia
{
    class PhysicsLayer: public LayerInterface
    {
    public:
        using SelfType = PhysicsLayer;
    public:
        PhysicsLayer();
        virtual ~PhysicsLayer() override;

        virtual void OnEvent(EventBase& event) override;
        virtual void OnUpdate() override;

    private:
        void _OnPhysicsSimulatorSetActive(Events::PhysicsSimulatirSetActive& e);
        void _OnPhysicsSimulatorReset(Events::PhysicsSimulatorReset& e);
        void _OnPhysicsSimulatorSetScene(Events::PhysicsSimulatorSetScene& e);
        void _OnPhysicsSimulatorSetEntity(Events::PhysicsSimulatorSetEntity& e);

    private:
        std::shared_ptr<PhysicsSimulator> _spPhysicsSimulator{};
    };
}