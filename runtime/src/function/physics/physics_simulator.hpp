#pragma once

#include<memory>
#include<set>
#include<unordered_map>

#include"core/base.hpp"
#include"platform/jolt/jolt_header.hpp"
#include"resource/component/physics_component/physics_component.hpp"

namespace Arcadia
{
    struct ARCADIA_API JphObjectLayerPairFilerImpl: JPH::ObjectLayerPairFilter
    {
    public:
        virtual auto ShouldCollide(JPH::ObjectLayer obj_1, JPH::ObjectLayer obj_2) const -> bool override;

    };

    struct ARCADIA_API JphBroadPhaseLayerImpl final: JPH::BroadPhaseLayerInterface
    {
    public:
        JphBroadPhaseLayerImpl();

        [[nodiscard]]
        virtual auto GetNumBroadPhaseLayers() const->JPH::uint override;

        [[nodiscard]]
        virtual auto GetBroadPhaseLayer(JPH::ObjectLayer layer) const->JPH::BroadPhaseLayer override;

        [[nodiscard]]
        virtual auto GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const -> const char*
        {
            return nullptr;
        }

    private:
        JPH::BroadPhaseLayer _ObjectToBroadPhase[Arcadia::JphObjectLayers::NumLayers];
    };

    struct ARCADIA_API JphObjectVsBroadPhaseLayerFilterImpl: JPH::ObjectVsBroadPhaseLayerFilter
    {
    public:
        virtual auto ShouldCollide(JPH::ObjectLayer obj, JPH::BroadPhaseLayer bp) const -> bool override;

    };

    struct ARCADIA_API PhysicsSimulator
    {
    public:
        ARCADIA_EXCEPTION(SubmitFail);
        ARCADIA_EXCEPTION(UnknownPhysicsComponent);

        using jph_body_id_umap_type = std::unordered_map<Arcadia::Uuid, JPH::BodyID>;
        using self_type = PhysicsSimulator;
    public:
        PhysicsSimulator();
        ~PhysicsSimulator();

        /// @brief Check whether the physcis simulator is in build
        [[nodiscard]]
        auto IsInBuild() const -> bool
        {
            return _InBuild;
        }

        /// @brief Start building the physics simulator
        /// @details This function signs that the physics simulator is in build
        /// @note This function can only be called when the physics simulator is not in build
        void Prepare();

        /// @brief Finish building the physcis simulator
        /// @note This function can only be called when the physics simulator is in build
        void Finalize();

        /// @brief Submit a physics component to the physics simulator
        /// @note This function can only be called when the physics simulator is in build
        void Submit(const Arcadia::PhysicsComponent& physics_comp);

        /// @brief Update physcis simulator for one step
        /// @note This function can only be called when the physics simulator is not in build
        void Update();

        /// @brief Quary the updated data of the physics component from the physcis simulator
        /// @throw unkonwn_physics_component if the physics component was not submitted before quary
        /// @note This function can only be called when the physics simulator is not in build
        void Quary(PhysicsComponent& physics_comp);

        /// @brief Reset the physics simulator, all caches will be cleared
        void Reset();

        [[nodiscard]]
        auto ShouldUpdate() const -> bool;
        void ShouldUpdate(bool should_update);

        [[nodiscard]]
        auto GetJphTempAllocatorSize() const->JPH::uint;
        void SetJphTempAllocatorSize(JPH::uint jph_temp_allocator_size);

        [[nodiscard]]
        auto GetJphPhysicsSystemUpdatesPerSecond() const -> int;
        void SetJphPhysicsSystemUpdatesPerSecond(int jph_physics_system_updates_per_second);

        [[nodiscard]]
        auto GetJphBodyIdUmap() const -> const jph_body_id_umap_type&;

    private:
        void _AssertFrameInBuild() const;
        void _AssertFrameNotInBuild() const;
    public:
    private:
        bool _InBuild{ false };

        bool _ShouldUpdate{ false };

        JPH::uint _JphTempAllocatorSize{ 10 * 1024 * 1024 };

        int _JphPhysicsSystemUpdatesPerSecond{ 60 };

        jph_body_id_umap_type _umapJphBodyId{};
        std::set<Arcadia::Uuid> _setSubmittedBodyInfo{};

        Arcadia::JphBroadPhaseLayerImpl _JphBroadPhaseLayer{};
        Arcadia::JphObjectVsBroadPhaseLayerFilterImpl _JphObjectVsBroadLayerFilter{};
        Arcadia::JphObjectLayerPairFilerImpl _JphObjectLayerPairFilter{};

        std::unique_ptr<JPH::PhysicsSystem> _upJphPhysicsSystem{};
    };
}
