#include "pch.hpp"
#include "physics_simulator.hpp"

#include<thread>

arcadia::physics_simulator::physics_simulator()
{
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory{};
    JPH::RegisterTypes();

    const unsigned int max_bodies = 65536;
    const unsigned int num_body_mutexes = 0;
    const unsigned int max_body_pair = 65535;
    const unsigned int max_contact_constraints = 10240;

    _jph_physics_system_uptr = std::make_unique<JPH::PhysicsSystem>();
    _jph_physics_system_uptr->Init(max_bodies, num_body_mutexes, max_body_pair, max_contact_constraints, _jph_broad_phase_layer, _jph_object_vs_broad_phase_layer_filter, _jph_object_layer_pair_filter);
}

arcadia::physics_simulator::~physics_simulator()
{
    JPH::UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}

void arcadia::physics_simulator::prepare()
{
    _assert_frame_not_in_build();
    _in_build = true;

    // Clear submitted body uuids
    _submitted_body_info_set.clear();
}

void arcadia::physics_simulator::finalize()
{
    _assert_frame_in_build();
    _in_build = false;

    auto& body_interface = _jph_physics_system_uptr->GetBodyInterface();
    for(auto iter = _jph_body_id_umap.begin(); iter != _jph_body_id_umap.end();)
    {
        if(!_submitted_body_info_set.contains(iter->first))
        {
            body_interface.RemoveBody(iter->second);
            body_interface.DestroyBody(iter->second);
            iter = _jph_body_id_umap.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    _jph_physics_system_uptr->OptimizeBroadPhase();
}

void arcadia::physics_simulator::submit(const arcadia::physics_component& physics_comp)
{
    _assert_frame_in_build();

    if(physics_comp.has_body_info())
    {
        const auto& [uuid, body_info] = physics_comp.get_identifiable_jph_body_info_initial();
        if(!_jph_body_id_umap.contains(uuid))
        {
            auto& jph_body_interface = _jph_physics_system_uptr->GetBodyInterface();
            JPH::ShapeRefC jph_shape_refc = arcadia::match<JPH::Shape*>(
                body_info.jph_shape_info,
                [&](const arcadia::physics_component::jph_box_shape_info& info)
            {
                return new JPH::BoxShape{ arcadia::to_jph_vec3(info.half_extent), info.convex_radius };
            },
                [&](const arcadia::physics_component::jph_capsule_shape_info& info)
            {
                return new JPH::CapsuleShape{ info.half_height_of_cylinder,info.radius };
            },
                [&](const arcadia::physics_component::jph_cylinder_shape_info& info)
            {
                return new JPH::CylinderShape{ info.half_height,info.radius,info.convex_radius };
            },
                [&](const arcadia::physics_component::jph_sphere_shape_info& info)
            {
                return new JPH::SphereShape{ info.radius };
            }
            );
            auto body_id = jph_body_interface.CreateAndAddBody(
                JPH::BodyCreationSettings{
                    jph_shape_refc,
                    arcadia::to_jph_vec3(body_info.position),
                    arcadia::to_jph_quat(body_info.rotation),
                    body_info.jph_motion_type,
                    body_info.jph_object_layer
                },
                JPH::EActivation::Activate
            );
            if(body_id.IsInvalid())
            {
                throw submit_fail{ std::format("Failed to create body; Its uuid is {}",uuid) };
            }
            _jph_body_id_umap.try_emplace(uuid, body_id);
        }
        _submitted_body_info_set.emplace(uuid);
    }
}

void arcadia::physics_simulator::update()
{
    if(!should_update)
    {
        return;
    }

    JPH::TempAllocatorImpl temp_allocator{ jph_temp_allocator_size };
    JPH::JobSystemThreadPool job_system_thread_pool{ JPH::cMaxPhysicsJobs,JPH::cMaxPhysicsBarriers,static_cast<int>(std::thread::hardware_concurrency() - 1) };


    _jph_physics_system_uptr->Update(1.f / jph_physics_system_updates_per_second, jph_physics_system_collision_steps_per_update, &temp_allocator, &job_system_thread_pool);
}

void arcadia::physics_simulator::quary(physics_component& physics_comp)
{
    _assert_frame_not_in_build();

    if(physics_comp.has_body_info())
    {
        const auto& [uuid, jph_body_info_initial] = physics_comp.get_identifiable_jph_body_info_initial();
        if(_jph_body_id_umap.contains(uuid))
        {
            const auto& body_interface = _jph_physics_system_uptr->GetBodyInterface();
            const auto& body_id = _jph_body_id_umap.at(uuid);

            auto& jph_body_info_ongoing = physics_comp.get_jph_body_info_ongoing();
            jph_body_info_ongoing.active = body_interface.IsActive(body_id);
            jph_body_info_ongoing.position = arcadia::from_jph_vec3(body_interface.GetPosition(body_id));
            jph_body_info_ongoing.rotation = arcadia::from_jph_quat(body_interface.GetRotation(body_id));
            jph_body_info_ongoing.linear_velocity = arcadia::from_jph_vec3(body_interface.GetLinearVelocity(body_id));
            jph_body_info_ongoing.angular_velocity = arcadia::from_jph_vec3(body_interface.GetAngularVelocity(body_id));
        }
        else
        {
            throw unknown_physics_component{ "Cannot quary component that has not been submitted" };
        }
    }
}

void arcadia::physics_simulator::clear()
{
    _jph_body_id_umap.clear();
    _submitted_body_info_set.clear();
}

void arcadia::physics_simulator::_assert_frame_in_build() const
{
    ARCADIA_ASSERT(_in_build && "Frame is not in build, did you call `prepare()`?");
}

void arcadia::physics_simulator::_assert_frame_not_in_build() const
{
    ARCADIA_ASSERT(!_in_build && "Frame is in build, did you call `finalize()`?");
}

auto arcadia::jph_object_layer_pair_filter_impl::ShouldCollide(JPH::ObjectLayer obj_1, JPH::ObjectLayer obj_2) const -> bool
{
    switch(obj_1)
    {
        case arcadia::jph_object_layers::non_moving:
        {
            return obj_2 == arcadia::jph_object_layers::moving; // Non moving only collides with moving
        }
        case arcadia::jph_object_layers::moving:
        {
            return true; // Moving collides with everything
        }
        default:
        {
            return false;
        }
    }
}

arcadia::jph_broad_phase_layer_impl::jph_broad_phase_layer_impl()
{
    _object_to_broad_phase[arcadia::jph_object_layers::non_moving] = arcadia::jph_broad_phase_layers::non_moving;
    _object_to_broad_phase[arcadia::jph_object_layers::moving] = arcadia::jph_broad_phase_layers::moving;
}

auto arcadia::jph_broad_phase_layer_impl::GetNumBroadPhaseLayers() const -> JPH::uint
{
    return arcadia::jph_broad_phase_layers::num_layers;
}

auto arcadia::jph_broad_phase_layer_impl::GetBroadPhaseLayer(JPH::ObjectLayer layer) const -> JPH::BroadPhaseLayer
{
    ARCADIA_ASSERT(layer < GetNumBroadPhaseLayers());
    return _object_to_broad_phase[layer];
}

auto arcadia::jph_object_vs_broad_phase_layer_filter_impl::ShouldCollide(JPH::ObjectLayer obj, JPH::BroadPhaseLayer bp) const -> bool
{
    switch(obj)
    {
        case arcadia::jph_object_layers::non_moving:
        {
            return bp == arcadia::jph_broad_phase_layers::moving;
        }
        case arcadia::jph_object_layers::moving:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}
