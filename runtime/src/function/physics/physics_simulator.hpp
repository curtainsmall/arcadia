#pragma once

#include<memory>
#include<set>
#include<unordered_map>

#include"core/base.hpp"
#include"platform/jolt/jolt_header.hpp"
#include"resource/scene.hpp"

struct JphObjectLayerPairFilerImpl: JPH::ObjectLayerPairFilter
{
public:
    virtual auto ShouldCollide(JPH::ObjectLayer obj_1, JPH::ObjectLayer obj_2) const -> bool override;
};

struct JphBroadPhaseLayerImpl final: JPH::BroadPhaseLayerInterface
{
public:
    JphBroadPhaseLayerImpl();

    [[nodiscard]]
    virtual auto GetNumBroadPhaseLayers() const->JPH::uint override;

    [[nodiscard]]
    virtual auto GetBroadPhaseLayer(JPH::ObjectLayer layer) const->JPH::BroadPhaseLayer override;

    [[nodiscard]]
    virtual auto GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const -> const char*
    {
        return nullptr;
    }

private:
    JPH::BroadPhaseLayer _object_to_broad_phase[jph_object_layers::num_layers];
};

struct JphObjectVsBroadPhaseLayerFilterImpl: JPH::ObjectVsBroadPhaseLayerFilter
{
public:
    virtual auto ShouldCollide(JPH::ObjectLayer obj, JPH::BroadPhaseLayer bp) const -> bool override;
};

struct PhysicsSimulator
{
public:
    using jph_body_id_storage_type = std::unordered_map<Uuid, JPH::BodyID>;
    using self_type = PhysicsSimulator;
public:
    PhysicsSimulator();
    ~PhysicsSimulator();

    /// @brief Start building the physics simulator
    /// @details This function signs that the physics simulator is in build
    /// @note This function can only be called when the physics simulator is not in build
    void prepare();

    /// @brief Finish building the physcis simulator
    /// @note This function can only be called when the physics simulator is in build
    void finalize();

    /// @brief Submit a entity to the physics simulator
    /// @param name Entity to submit
    /// @note Entity that does not have physics component will be ignored
    /// @note This function can only be called when the physics simulator is in build
    void submit(const Scene& scene, const std::string& name);

    /// @brief Update physcis simulator for one step
    /// @note This function can only be called when the physics simulator is not in build
    void update();

    /// @brief Query the updated data of the physics component from the physcis simulator
    /// @param name Entity to quary
    /// @note Entity that does not have physics component will be ignored
    /// @note This function can only be called when the physics simulator is not in build
    void query(Scene& scene, const std::string& name);

    /// @brief Reset the physics simulator, all caches will be cleared
    void reset();

    [[nodiscard]]
    auto is_active() const -> bool;
    void set_active(bool should_update);

    [[nodiscard]]
    auto get_jph_temp_allocator_size() const->JPH::uint;
    void set_jph_temp_allocator_size(JPH::uint get_jph_temp_allocator_size);

    [[nodiscard]]
    auto get_jph_physics_system_updates_per_second() const -> int;
    void set_jph_physics_system_updates_per_second(int jph_physics_system_updates_per_second);

    [[nodiscard]]
    auto jph_body_id_storage() const -> const jph_body_id_storage_type&;

private:
    void _assert_frame_in_build() const;
    void _assert_frame_not_in_build() const;
public:
private:
    bool _in_build{ false };

    bool _active{ false };

    JPH::uint _jph_temp_allocator_size{ 10 * 1024 * 1024 };

    int _jph_physics_system_updates_per_second{ 60 };

    jph_body_id_storage_type _jph_body_id_storage{};
    std::set<Uuid> _submitted_body_infos{};

    JphBroadPhaseLayerImpl _jph_broad_phase_layer{};
    JphObjectVsBroadPhaseLayerFilterImpl _jph_object_vs_broad_layer_filter{};
    JphObjectLayerPairFilerImpl _jph_object_layer_pair_filter{};

    std::unique_ptr<JPH::PhysicsSystem> _jph_physics_system{};
};
