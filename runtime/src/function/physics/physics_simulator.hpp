#pragma once

#include<memory>
#include<set>
#include<unordered_map>

#include"platform/api_def.hpp"
#include"platform/jolt/jolt_header.hpp"
#include"resource/scene.hpp"

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

    class PhysicsSimulator
    {
    public:
        using JphBodyIdStorageType = std::unordered_map<Uuid, JPH::BodyID>;
        using SelfType = PhysicsSimulator;
    public:
        PhysicsSimulator();
        ~PhysicsSimulator();

        /// @brief Start building the physics simulator
        /// @details This function signs that the physics simulator is in build
        /// @note This function can only be called when the physics simulator is not in build
        void Prepare();

        /// @brief Finish building the physcis simulator
        /// @note This function can only be called when the physics simulator is in build
        void Finalize();

        /// @brief Submit a entity to the physics simulator
        /// @param name Entity to submit
        /// @note Entity that does not have physics component will be ignored
        /// @note This function can only be called when the physics simulator is in build
        void Submit(const Scene& scene, const std::string& name);

        /// @brief Update physcis simulator for one step
        /// @note This function can only be called when the physics simulator is not in build
        void Update();

        /// @brief Query the updated data of the physics component from the physcis simulator
        /// @param name Entity to quary
        /// @note Entity that does not have physics component will be ignored
        /// @note This function can only be called when the physics simulator is not in build
        void Query(Scene& scene, const std::string& name);

        /// @brief Reset the physics simulator, all caches will be cleared
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
        auto GetJphBodyIdStorage() const -> const JphBodyIdStorageType&;

    private:
        void _AssertFrameInBuild() const;
        void _AssertFrameNotInBuild() const;
    public:
    private:
        bool _InBuild{ false };

        bool _Active{ false };

        JPH::uint _JphTempAllocatorSize{ 10 * 1024 * 1024 };

        std::int32_t _JphPhysicsSystemUpdatesPerSecond{ 60 };

        JphBodyIdStorageType _JphBodyIdStorage{};
        std::set<Uuid> _SubmittedBodyInfos{};

        JphBroadPhaseLayerImpl _JphBroadPhaseLayer{};
        JphObjectVsBroadPhaseLayerFilterImpl _JphObjectVsBroadLayerFilter{};
        JphObjectLayerPairFilerImpl _JphObjectLayerPairFilter{};

        std::unique_ptr<JPH::PhysicsSystem> _JphPhysicsSystemUniquePtr{};
    };
}