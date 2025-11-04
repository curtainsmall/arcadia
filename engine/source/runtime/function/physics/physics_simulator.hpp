#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "platform/defines.hpp"
#include "platform/jolt.hpp"
#include "resource/components/physics_component.hpp"
#include "resource/components/transform_component.hpp"
#include "resource/scene.hpp"

namespace Arcadia
{
    struct ACDA_API JphObjectLayerPairFilerImpl: public JPH::ObjectLayerPairFilter
    {
    public:
        virtual auto ShouldCollide(JPH::ObjectLayer obj_1, JPH::ObjectLayer obj_2) const -> bool override;
    };

    struct ACDA_API JphBroadPhaseLayerImpl: public JPH::BroadPhaseLayerInterface
    {
    public:
        JphBroadPhaseLayerImpl();

        [[nodiscard]]
        virtual auto GetNumBroadPhaseLayers() const -> JPH::uint override;

        [[nodiscard]]
        virtual auto GetBroadPhaseLayer(JPH::ObjectLayer layer) const -> JPH::BroadPhaseLayer override;

        [[nodiscard]]
        virtual auto GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const -> const char* override
        {
            return nullptr;
        }

    private:
        JPH::BroadPhaseLayer _ObjectToBroadPhase[JphObjectLayers::NumLayers];
    };

    struct ACDA_API JphObjectVsBroadPhaseLayerFilterImpl: public JPH::ObjectVsBroadPhaseLayerFilter
    {
    public:
        virtual auto ShouldCollide(JPH::ObjectLayer obj, JPH::BroadPhaseLayer bp) const -> bool override;
    };

    struct ACDA_API PhysicsSimulator
    {
    public:
        using SelfType = PhysicsSimulator;
    public:
        PhysicsSimulator();
        ~PhysicsSimulator();

        PhysicsSimulator(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;

        [[nodiscard]]
        auto HasEntity(EntityId entity_id) const -> bool;
        void BuildEntity(EntityId entity_id);
        void RemoveEntity(EntityId entity_id);

        // Update physics simulation
        void Update();

        // Apply changes to entities
        void ApplyToEntites();

        // Reset physics simulator, clear all bodies
        void Reset();

        [[nodiscard]]
        auto IsActive() const -> bool;
        void SetActive(bool active);

        [[nodiscard]]
        auto GetJphTempAllocatorSize() const -> JPH::uint;
        void SetJphTempAllocatorSize(JPH::uint jph_temp_allocator_size);

        [[nodiscard]]
        auto GetJphPhysicsSystemUpdatesPerSecond() const -> std::int32_t;
        void SetJphPhysicsSystemUpdatesPerSecond(std::int32_t jph_physics_system_updates_per_second);

        [[nodiscard]]
        auto GetBodyCount() const -> std::size_t;

    private:
        void _Clear();

        auto _CreateJphShape(const PhysicsComponent& physics_comp) const -> const JPH::Shape*;
        void _CreateJphBody(
            JPH::BodyInterface& jph_body_interface,
            const PhysicsComponent& physics_comp,
            const TransformComponent& transform_comp,
            EntityId entity_id
        );
        void _UpdateJphBody(
            JPH::BodyInterface& jph_body_interface,
            const PhysicsComponent& physics_comp,
            const TransformComponent& transform_comp,
            EntityId entity_id
        );
    private:

        bool _Active{ false };

        JPH::uint _JphTempAllocatorSize{ 10 * 1024 * 1024 };
        std::int32_t _JphPhysicsSystemUpdatesPerSecond{ 60 };

        std::unordered_map<EntityId, JPH::BodyID> _JphBodyIdStorage{};
        std::unordered_set<EntityId> _EntityIdSet{};

        JphBroadPhaseLayerImpl _JphBroadPhaseLayer{};
        JphObjectVsBroadPhaseLayerFilterImpl _JphObjectVsBroadLayerFilter{};
        JphObjectLayerPairFilerImpl _JphObjectLayerPairFilter{};

        JPH::PhysicsSystem _JphPhysicsSystem{};
    };
}