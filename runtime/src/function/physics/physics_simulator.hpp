#pragma once

#include<memory>
#include<set>
#include<unordered_map>

#include"core/base.hpp"
#include"platform/jolt/jolt_header.hpp"
#include"resource/component/physics_component/physics_component.hpp"

namespace arcadia
{
    struct ARCADIA_API jph_object_layer_pair_filter_impl: JPH::ObjectLayerPairFilter
    {
    public:
        virtual auto ShouldCollide(JPH::ObjectLayer obj_1, JPH::ObjectLayer obj_2) const -> bool override;

    };

    struct ARCADIA_API jph_broad_phase_layer_impl final: JPH::BroadPhaseLayerInterface
    {
    public:
        jph_broad_phase_layer_impl();

        [[nodiscard]]
        virtual auto GetNumBroadPhaseLayers() const->JPH::uint override;

        [[nodiscard]]
        virtual auto GetBroadPhaseLayer(JPH::ObjectLayer layer) const->JPH::BroadPhaseLayer override;

        [[nodiscard]]
        virtual inline auto GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const -> const char*
        {
            return nullptr;
        }

    private:
        JPH::BroadPhaseLayer _object_to_broad_phase[arcadia::jph_object_layers::num_layers];
    };

    struct ARCADIA_API jph_object_vs_broad_phase_layer_filter_impl: JPH::ObjectVsBroadPhaseLayerFilter
    {
    public:
        virtual auto ShouldCollide(JPH::ObjectLayer obj, JPH::BroadPhaseLayer bp) const -> bool override;

    };

    struct ARCADIA_API physics_simulator
    {
    public:
        ARCADIA_EXCEPTION(submit_fail);
        ARCADIA_EXCEPTION(unknown_physics_component);

        using jph_body_id_umap_type = std::unordered_map<arcadia::uuid, JPH::BodyID>;
        using self_type = physics_simulator;
    public:
        physics_simulator();
        ~physics_simulator();

        /// @brief Check whether the physcis simulator is in build
        [[nodiscard]]
        inline auto is_in_build() const -> bool
        {
            return _in_build;
        }

        /// @brief Start building the physics simulator
        /// @details This function signs that the physics simulator is in build
        /// @note This function can only be called when the physics simulator is not in build
        void prepare();

        /// @brief Finish building the physcis simulator
        /// @note This function can only be called when the physics simulator is in build
        void finalize();

        /// @brief Submit a physics component to the physics simulator
        /// @note This function can only be called when the physics simulator is in build
        void submit(const arcadia::physics_component& physics_comp);

        /// @brief Update physcis simulator for one step
        /// @note This function can only be called when the physics simulator is not in build
        void update();

        /// @brief Quary the updated data of the physics component from the physcis simulator
        /// @throw unkonwn_physics_component if the physics component was not submitted before quary
        /// @note This function can only be called when the physics simulator is not in build
        void quary(physics_component& physics_comp);

        /// @brief Reset the physics simulator, all caches will be cleared
        void reset();

        [[nodiscard]]
        inline auto get_jph_body_id_umap() const -> const jph_body_id_umap_type&
        {
            return _jph_body_id_umap;
        }

    private:
        void _assert_frame_in_build() const;
        void _assert_frame_not_in_build() const;
    public:
        bool should_update{ false };

        JPH::uint jph_temp_allocator_size{ 10 * 1024 * 1024 };

        int jph_physics_system_updates_per_second{ 60 };
        int jph_physics_system_collision_steps_per_update{ 1 };
    private:
        bool _in_build{ false };

        jph_body_id_umap_type _jph_body_id_umap{};
        std::set<arcadia::uuid> _submitted_body_info_set{};

        arcadia::jph_broad_phase_layer_impl _jph_broad_phase_layer{};
        arcadia::jph_object_vs_broad_phase_layer_filter_impl _jph_object_vs_broad_phase_layer_filter{};
        arcadia::jph_object_layer_pair_filter_impl _jph_object_layer_pair_filter{};

        std::unique_ptr<JPH::PhysicsSystem> _jph_physics_system_uptr{};
    };
}
