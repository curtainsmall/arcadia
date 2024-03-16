#pragma once

#include"Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h"
#include"Jolt/Physics/Collision/ObjectLayer.h"
#include"Jolt/Physics/PhysicsSystem.h"

#include"core/base.hpp"
#include"platform/jolt/jolt_header.hpp"

namespace arcadia
{
    namespace object_layers
    {
        static constexpr JPH::ObjectLayer non_moving{ 0 };
        static constexpr JPH::ObjectLayer moving{ 1 };
        static constexpr std::size_t num_layers{ 2 };
    }

    struct ARCADIA_API object_layer_pair_filter_impl: JPH::ObjectLayerPairFilter
    {
    public:
        virtual auto ShouldCollide(JPH::ObjectLayer obj_1, JPH::ObjectLayer obj_2) const -> bool override;

    };

    namespace broad_phase_layers
    {
        static constexpr JPH::BroadPhaseLayer non_moving{ 0 };
        static constexpr JPH::BroadPhaseLayer moving{ 0 };
        static constexpr std::size_t num_layers{ 2 };
    }

    struct ARCADIA_API broad_phase_layer_impl final: JPH::BroadPhaseLayerInterface
    {
    public:
        broad_phase_layer_impl();

        [[nodiscard]]
        virtual auto GetNumBroadPhaseLayers() const->JPH::uint override;

        [[nodiscard]]
        virtual auto GetBroadPhaseLayer(JPH::ObjectLayer layer) const->JPH::BroadPhaseLayer override;

    private:
        JPH::BroadPhaseLayer _object_to_broad_phase[arcadia::object_layers::num_layers];
    };

    struct ARCADIA_API object_vs_broad_phase_layer_filter_impl: JPH::ObjectVsBroadPhaseLayerFilter
    {
    public:
        virtual auto ShouldCollide(JPH::ObjectLayer obj, JPH::BroadPhaseLayer bp) const -> bool override;

    };

    struct ARCADIA_API physics_simulator
    {
    public:
        using self_type = physics_simulator;
    public:
        physics_simulator();
        ~physics_simulator();

    private:
        arcadia::broad_phase_layer_impl _broad_phase_layer{};
        arcadia::object_vs_broad_phase_layer_filter_impl _object_vs_broad_phase_layer_filter{};
        arcadia::object_layer_pair_filter_impl _object_layer_pair_filter{};

        JPH::PhysicsSystem  _physics_system{};
    };
}
