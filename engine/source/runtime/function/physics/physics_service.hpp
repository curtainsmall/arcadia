#pragma once

#include <memory>

#include "core/event.hpp"
#include "app/engine.hpp"
#include "app/services.hpp"
#include "function/physics/physics_events.hpp"
#include "function/physics/physics_simulator.hpp"

namespace Arcadia
{
    struct ACDA_API PhysicsService: public Physics
    {
    public:
        using SelfType = PhysicsService;
    public:
        PhysicsService(Engine& engine);
        virtual ~PhysicsService() override = default;

        void Step(std::chrono::nanoseconds delta_time) override;
        void ApplyToWorld() override;

        void OnEvent(EventBase& event);

        [[nodiscard]]
        auto IsPhysicsSimulatorActive() const -> bool;

        [[nodiscard]]
        auto GetPhysicsBodyCount() const -> std::size_t;

        [[nodiscard]]
        auto GetTempAllocatorSize() const -> std::size_t;

        [[nodiscard]]
        auto GetUpdatesPerSecondCount() const -> std::int32_t;

    private:
        void _OnPhysicsSimulatorReset(Events::PhysicsSimulatorReset& e);
        void _OnPhysicsSimulatorSetActive(Events::PhysicsSimulatorSetActive& e);
        void _OnPhysicsSimulatorSetEntity(Events::PhysicsSimulatorSetEntity& e);

    private:
        Engine& _Engine;

        Foundation::Subscription _EventSubscription{};

        PhysicsSimulator _PhysicsSimulator;
    };
}
