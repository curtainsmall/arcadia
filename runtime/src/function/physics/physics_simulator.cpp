#include "pch.hpp"
#include "physics_simulator.hpp"

#include"Jolt/Core/Factory.h"
#include"Jolt/RegisterTypes.h"

arcadia::physics_simulator::physics_simulator()
{
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory{};
    JPH::RegisterTypes();

    const unsigned int max_bodies = 65536;
    const unsigned int num_body_mutexes = 0;
    const unsigned int max_body_pair = 65535;
    const unsigned int max_contact_constraints = 10240;

    _physics_system.Init(max_bodies, num_body_mutexes, max_body_pair, max_contact_constraints, _broad_phase_layer, _object_vs_broad_phase_layer_filter, _object_layer_pair_filter);
}

arcadia::physics_simulator::~physics_simulator()
{
    JPH::UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}

auto arcadia::object_layer_pair_filter_impl::ShouldCollide(JPH::ObjectLayer obj_1, JPH::ObjectLayer obj_2) const -> bool
{
    switch(obj_1)
    {
        case arcadia::object_layers::non_moving:
        {
            return obj_2 == arcadia::object_layers::moving; // Non moving only collides with moving
        }
        case arcadia::object_layers::moving:
        {
            return true; // Moving collides with everything
        }
        default:
        {
            return false;
        }
    }
}

arcadia::broad_phase_layer_impl::broad_phase_layer_impl()
{
    _object_to_broad_phase[arcadia::object_layers::non_moving] = arcadia::broad_phase_layers::non_moving;
    _object_to_broad_phase[arcadia::object_layers::moving] = arcadia::broad_phase_layers::moving;
}

auto arcadia::broad_phase_layer_impl::GetNumBroadPhaseLayers() const -> JPH::uint
{
    return arcadia::broad_phase_layers::num_layers;
}

auto arcadia::broad_phase_layer_impl::GetBroadPhaseLayer(JPH::ObjectLayer layer) const -> JPH::BroadPhaseLayer
{
    ARCADIA_ASSERT(layer < GetNumBroadPhaseLayers());
    return _object_to_broad_phase[layer];
}

auto arcadia::object_vs_broad_phase_layer_filter_impl::ShouldCollide(JPH::ObjectLayer obj, JPH::BroadPhaseLayer bp) const -> bool
{
    switch(obj)
    {
        case arcadia::object_layers::non_moving:
        {
            return bp == arcadia::broad_phase_layers::moving;
        }
        case arcadia::object_layers::moving:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}
