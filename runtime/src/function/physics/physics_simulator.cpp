#include "pch.hpp"

#include "physics_simulator.hpp"

#include<thread>

#include"resource/components/physics_component.hpp"
#include"resource/components/transform_component.hpp"

PhysicsSimulator::PhysicsSimulator()
{
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory{};
    JPH::RegisterTypes();

    const unsigned int max_bodies = 65536;
    const unsigned int num_body_mutexes = 0;
    const unsigned int max_body_pair = 65535;
    const unsigned int max_contact_constraints = 10240;

    _jph_physics_system = std::make_unique<JPH::PhysicsSystem>();
    _jph_physics_system->Init(max_bodies, num_body_mutexes, max_body_pair, max_contact_constraints, _jph_broad_phase_layer, _jph_object_vs_broad_layer_filter, _jph_object_layer_pair_filter);
}

PhysicsSimulator::~PhysicsSimulator()
{
    JPH::UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}

void PhysicsSimulator::prepare()
{
    _assert_frame_not_in_build();
    _in_build = true;

    // Clear submitted body uuids
    _submitted_body_infos.clear();
}

void PhysicsSimulator::finalize()
{
    _assert_frame_in_build();
    _in_build = false;

    auto& jph_body_interface = _jph_physics_system->GetBodyInterface();
    for(auto iter = _jph_body_id_storage.begin(); iter != _jph_body_id_storage.end();)
    {
        if(!_submitted_body_infos.contains(iter->first))
        {
            jph_body_interface.RemoveBody(iter->second);
            jph_body_interface.DestroyBody(iter->second);
            iter = _jph_body_id_storage.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    _jph_physics_system->OptimizeBroadPhase();
}

void PhysicsSimulator::submit(const Scene& scene, const std::string& name)
{
    _assert_frame_in_build();

    const auto& entity_info = scene.entity_info(name);

    // For now, only actor entity has physics component
    if(entity_info.type != "actor")
    {
        return;
    }

    const auto [physics_comp, transform_comp] = scene.get<PhysicsComponent, TransformComponent>(name);

    if(physics_comp.has_body_info())
    {
        const auto& [uuid, body_info] = physics_comp.get_identifiable_jph_body_info();
        if(!_jph_body_id_storage.contains(uuid))
        {
            auto& jph_body_interface = _jph_physics_system->GetBodyInterface();
            JPH::ShapeRefC jph_shape_refc = match<JPH::Shape*>(
                body_info.jph_shape_info,
                [&](const JphBoxShapeInfo& info)
            {
                return new JPH::BoxShape{ to_jph_vec3(info.half_extent), info.convex_radius };
            },
                [&](const JphCapsuleShapeInfo& info)
            {
                return new JPH::CapsuleShape{ info.half_height_of_cylinder,info.radius };
            },
                [&](const JphCylinderShapeInfo& info)
            {
                return new JPH::CylinderShape{ info.half_height,info.radius,info.convex_radius };
            },
                [&](const JphSphereShapeInfo& info)
            {
                return new JPH::SphereShape{ info.radius };
            }
            );
            const auto body_id = jph_body_interface.CreateAndAddBody(
                JPH::BodyCreationSettings{
                    jph_shape_refc,
                    to_jph_vec3(transform_comp.position),
                    to_jph_quat(transform_comp.rotation),
                    body_info.jph_motion_type,
                    body_info.jph_object_layer
                },
                JPH::EActivation::Activate
            );
            ACDA_ASSERT(!body_id.IsInvalid() && "Failed to create body");
            _jph_body_id_storage.try_emplace(uuid, body_id);
        }
        _submitted_body_infos.emplace(uuid);
    }
}

void PhysicsSimulator::update()
{
    if(!_active)
    {
        return;
    }

    JPH::TempAllocatorImpl temp_allocator{ _jph_temp_allocator_size };
    JPH::JobSystemThreadPool job_system_thread_pool{ JPH::cMaxPhysicsJobs,JPH::cMaxPhysicsBarriers,static_cast<int>(std::thread::hardware_concurrency() - 1) };


    _jph_physics_system->Update(1.f / _jph_physics_system_updates_per_second, 60 / _jph_physics_system_updates_per_second, &temp_allocator, &job_system_thread_pool);
}

void PhysicsSimulator::query(Scene& scene, const std::string& name)
{
    _assert_frame_not_in_build();

    if(!_active)
    {
        return;
    }

    const auto& entity_info = scene.entity_info(name);

    // For now, only actor entity has physics component
    if(entity_info.type != "actor")
    {
        return;
    }

    auto [physics_comp, transform_comp] = scene.get<PhysicsComponent, TransformComponent>(name);

    if(physics_comp.has_body_info())
    {
        const auto& [uuid, jph_body_info_initial] = physics_comp.get_identifiable_jph_body_info();
        ACDA_ASSERT(_jph_body_id_storage.contains(uuid));
        const auto& jph_body_interface = _jph_physics_system->GetBodyInterface();
        const auto& body_id = _jph_body_id_storage.at(uuid);

        auto& jph_body_state = physics_comp.jph_body_state;
        jph_body_state.active = jph_body_interface.IsActive(body_id);
        jph_body_state.linear_velocity = from_jph_vec3(jph_body_interface.GetLinearVelocity(body_id));
        jph_body_state.angular_velocity = from_jph_vec3(jph_body_interface.GetAngularVelocity(body_id));

        transform_comp.position = from_jph_vec3(jph_body_interface.GetPosition(body_id));
        transform_comp.rotation = from_jph_quat(jph_body_interface.GetRotation(body_id));
    }
}

void PhysicsSimulator::reset()
{
    auto& jph_body_interface = _jph_physics_system->GetBodyInterface();
    for(const auto& [uuid, body_id] : _jph_body_id_storage)
    {
        jph_body_interface.RemoveBody(body_id);
        jph_body_interface.DestroyBody(body_id);
    }
    _jph_body_id_storage.clear();
    _submitted_body_infos.clear();
}

auto PhysicsSimulator::is_active() const -> bool
{
    return _active;
}

void PhysicsSimulator::set_active(bool should_update)
{
    _active = should_update;
}

auto PhysicsSimulator::get_jph_temp_allocator_size() const -> JPH::uint
{
    return _jph_temp_allocator_size;
}

void PhysicsSimulator::set_jph_temp_allocator_size(JPH::uint get_jph_temp_allocator_size)
{
    _jph_temp_allocator_size = get_jph_temp_allocator_size;
}

auto PhysicsSimulator::get_jph_physics_system_updates_per_second() const -> int
{
    return _jph_physics_system_updates_per_second;
}

void PhysicsSimulator::set_jph_physics_system_updates_per_second(int jph_physics_system_updates_per_second)
{
    _jph_physics_system_updates_per_second = jph_physics_system_updates_per_second;
}

auto PhysicsSimulator::jph_body_id_storage() const -> const jph_body_id_storage_type&
{
    return _jph_body_id_storage;
}

void PhysicsSimulator::_assert_frame_in_build() const
{
    ACDA_ASSERT(_in_build && "Frame is not in build, did you call `prepare()`?");
}

void PhysicsSimulator::_assert_frame_not_in_build() const
{
    ACDA_ASSERT(!_in_build && "Frame is in build, did you call `finalize()`?");
}

auto JphObjectLayerPairFilerImpl::ShouldCollide(JPH::ObjectLayer obj_1, JPH::ObjectLayer obj_2) const -> bool
{
    switch(obj_1)
    {
        case jph_object_layers::non_moving:
        {
            return obj_2 == jph_object_layers::moving; // Non moving only collides with moving
        }
        case jph_object_layers::moving:
        {
            return true; // Moving collides with everything
        }
        default:
        {
            return false;
        }
    }
}

JphBroadPhaseLayerImpl::JphBroadPhaseLayerImpl()
{
    _object_to_broad_phase[jph_object_layers::non_moving] = jph_broad_phase_layers::non_moving;
    _object_to_broad_phase[jph_object_layers::moving] = jph_broad_phase_layers::moving;
}

auto JphBroadPhaseLayerImpl::GetNumBroadPhaseLayers() const -> JPH::uint
{
    return jph_broad_phase_layers::num_layers;
}

auto JphBroadPhaseLayerImpl::GetBroadPhaseLayer(JPH::ObjectLayer layer) const -> JPH::BroadPhaseLayer
{
    ACDA_ASSERT(layer < GetNumBroadPhaseLayers());
    return _object_to_broad_phase[layer];
}

auto JphObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(JPH::ObjectLayer obj, JPH::BroadPhaseLayer bp) const -> bool
{
    switch(obj)
    {
        case jph_object_layers::non_moving:
        {
            return bp == jph_broad_phase_layers::moving;
        }
        case jph_object_layers::moving:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}
