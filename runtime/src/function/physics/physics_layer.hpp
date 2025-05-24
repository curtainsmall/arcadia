#pragma once

#include <memory>

#include "core/event.hpp"
#include "core/layer.hpp"
#include "function/physics/physics_events.hpp"
#include "function/physics/physics_simulator.hpp"

namespace Arcadia
{
    struct PhysicsLayer: public LayerInterface
    {
    public:
        using SelfType = PhysicsLayer;
    public:
        PhysicsLayer();
        virtual ~PhysicsLayer() override = default;

        virtual void OnEvent(EventBase& event) override;
        virtual void OnUpdate() override;

        [[nodiscard]]
        auto IsPhysicsSimulatorActive() const -> bool;

        [[nodiscard]]
        auto GetPhysicsBodyCount() const->std::size_t;

        [[nodiscard]]
        auto GetTempAllocatorSize() const->std::size_t;

        [[nodiscard]]
        auto GetUpdatesPerSecondCount() const->std::int32_t;

    private:
        void _OnPhysicsSimulatorReset(Events::PhysicsSimulatorReset& e);
        void _OnPhysicsSimulatorSetActive(Events::PhysicsSimulatirSetActive& e);
        void _OnPhysicsSimulatorSetEntity(Events::PhysicsSimulatorSetEntity& e);

    private:
        std::shared_ptr<PhysicsSimulator> _spPhysicsSimulator{};
    };
}