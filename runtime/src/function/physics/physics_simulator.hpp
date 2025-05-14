#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "core/noncopyable.hpp"
#include "platform/api_def.hpp"
#include "platform/jolt.hpp"
#include "resource/scene.hpp"

namespace Arcadia
{
    class JphObjectLayerPairFilerImpl: public JPH::ObjectLayerPairFilter
    {
    public:
        virtual auto ShouldCollide(JPH::ObjectLayer obj_1, JPH::ObjectLayer obj_2) const -> bool override;
    };

    class JphBroadPhaseLayerImpl: public JPH::BroadPhaseLayerInterface
    {
    public:
        JphBroadPhaseLayerImpl();

        [[nodiscard]]
        virtual auto GetNumBroadPhaseLayers() const->JPH::uint override;

        [[nodiscard]]
        virtual auto GetBroadPhaseLayer(JPH::ObjectLayer layer) const->JPH::BroadPhaseLayer override;

        [[nodiscard]]
        virtual auto GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const -> const char* override
        {
            return nullptr;
        }

    private:
        JPH::BroadPhaseLayer _ObjectToBroadPhase[JphObjectLayers::NumLayers];
    };

    class JphObjectVsBroadPhaseLayerFilterImpl: public JPH::ObjectVsBroadPhaseLayerFilter
    {
    public:
        virtual auto ShouldCollide(JPH::ObjectLayer obj, JPH::BroadPhaseLayer bp) const -> bool override;
    };

    class PhysicsSimulator: public Noncopyable
    {
    public:
        using SelfType = PhysicsSimulator;
    public:
        PhysicsSimulator();
        ~PhysicsSimulator();

        [[nodiscard]]
        auto HasEntity(EntityId entity_id) const -> bool;
        void AddEntity(EntityId entity_id);
        void RemoveEntity(EntityId entity_id);
        void UpdateEntity(EntityId entity_id);
        void Update();
        void Reset();

        [[nodiscard]]
        auto IsActive() const -> bool;
        void SetActive(bool should_update);

        [[nodiscard]]
        auto GetJphTempAllocatorSize() const->JPH::uint;
        void SetJphTempAllocatorSize(JPH::uint get_jph_temp_allocator_size);

        [[nodiscard]]
        auto GetJphPhysicsSystemUpdatesPerSecond() const->std::int32_t;
        void SetJphPhysicsSystemUpdatesPerSecond(std::int32_t jph_physics_system_updates_per_second);

        [[nodiscard]]
        auto GetBodyCount() const->std::size_t;

    private:
        void _BuildForEntity(EntityId entity_id, bool update_hint);
        void _ClearForEntity(EntityId entity_id);
        void _Clear();
    private:

        bool _Active{ false };

        JPH::uint _JphTempAllocatorSize{ 10 * 1024 * 1024 };
        std::int32_t _JphPhysicsSystemUpdatesPerSecond{ 60 };

        std::unordered_map<EntityId, JPH::BodyID> _JphBodyIdStorage{};
        std::unordered_set<EntityId> _EntityIdSet{};

        JphBroadPhaseLayerImpl _JphBroadPhaseLayer{};
        JphObjectVsBroadPhaseLayerFilterImpl _JphObjectVsBroadLayerFilter{};
        JphObjectLayerPairFilerImpl _JphObjectLayerPairFilter{};

        std::unique_ptr<JPH::PhysicsSystem> _upJphPhysicsSystemUniquePtr{};
    };
}