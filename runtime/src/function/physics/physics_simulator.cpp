#include "pch.hpp"

#include "physics_simulator.hpp"

#include<thread>

#include"resource/components/physics_component.hpp"
#include"resource/components/transform_component.hpp"

Arcadia::PhysicsSimulator::PhysicsSimulator()
{
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory{};
    JPH::RegisterTypes();

    const unsigned int max_bodies = 65536;
    const unsigned int num_body_mutexes = 0;
    const unsigned int max_body_pair = 65535;
    const unsigned int max_contact_constraints = 10240;

    _JphPhysicsSystem = std::make_unique<JPH::PhysicsSystem>();
    _JphPhysicsSystem->Init(max_bodies, num_body_mutexes, max_body_pair, max_contact_constraints, _JphBroadPhaseLayer, _JphObjectVsBroadLayerFilter, _JphObjectLayerPairFilter);
}

Arcadia::PhysicsSimulator::~PhysicsSimulator()
{
    JPH::UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}

void Arcadia::PhysicsSimulator::Prepare()
{
    _AssertFrameNotInBuild();
    _InBuild = true;

    // Clear submitted body uuids
    _SubmittedBodyInfos.clear();
}

void Arcadia::PhysicsSimulator::Finalize()
{
    _AssertFrameInBuild();
    _InBuild = false;

    auto& jph_body_interface = _JphPhysicsSystem->GetBodyInterface();
    for(auto iter = _JphBodyIdStorage.begin(); iter != _JphBodyIdStorage.end();)
    {
        if(!_SubmittedBodyInfos.contains(iter->first))
        {
            jph_body_interface.RemoveBody(iter->second);
            jph_body_interface.DestroyBody(iter->second);
            iter = _JphBodyIdStorage.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    _JphPhysicsSystem->OptimizeBroadPhase();
}

void Arcadia::PhysicsSimulator::Submit(const Arcadia::Scene& scene, const std::string& name)
{
    _AssertFrameInBuild();

    const auto& entity_info = scene.GetEntityInfo(name);

    // For now, only actor entity has physics component
    if(entity_info.Type != "actor")
    {
        return;
    }

    const auto [physics_comp, transform_comp] = scene.Get<Arcadia::PhysicsComponent, Arcadia::TransformComponent>(name);

    if(physics_comp.HasBodyInfo())
    {
        const auto& [uuid, body_info] = physics_comp.GetIdentifiableJphBodyInfo();
        if(!_JphBodyIdStorage.contains(uuid))
        {
            auto& jph_body_interface = _JphPhysicsSystem->GetBodyInterface();
            JPH::ShapeRefC jph_shape_refc = Arcadia::Match<JPH::Shape*>(
                body_info.JphShapeInfo,
                [&](const Arcadia::JphBoxShapeInfo& info)
            {
                return new JPH::BoxShape{ Arcadia::ToJphVec3(info.HalfExtent), info.ConvexRadius };
            },
                [&](const Arcadia::JphCapsuleShapeInfo& info)
            {
                return new JPH::CapsuleShape{ info.HalfHeightOfCylinder,info.Radius };
            },
                [&](const Arcadia::JphCylinderShapeInfo& info)
            {
                return new JPH::CylinderShape{ info.HalfHeight,info.Radius,info.ConvexRadius };
            },
                [&](const Arcadia::JphSphereShapeInfo& info)
            {
                return new JPH::SphereShape{ info.Radius };
            }
            );
            const auto body_id = jph_body_interface.CreateAndAddBody(
                JPH::BodyCreationSettings{
                    jph_shape_refc,
                    Arcadia::ToJphVec3(transform_comp.Position),
                    Arcadia::ToJphQuat(transform_comp.Rotation),
                    body_info.JphMotionType,
                    body_info.JphObjectLayer
                },
                JPH::EActivation::Activate
            );
            ARCADIA_ASSERT(!body_id.IsInvalid() && "Failed to create body");
            _JphBodyIdStorage.try_emplace(uuid, body_id);
        }
        _SubmittedBodyInfos.emplace(uuid);
    }
}

void Arcadia::PhysicsSimulator::Update()
{
    if(!_ShouldUpdate)
    {
        return;
    }

    JPH::TempAllocatorImpl temp_allocator{ _JphTempAllocatorSize };
    JPH::JobSystemThreadPool job_system_thread_pool{ JPH::cMaxPhysicsJobs,JPH::cMaxPhysicsBarriers,static_cast<int>(std::thread::hardware_concurrency() - 1) };


    _JphPhysicsSystem->Update(1.f / _JphPhysicsSystemUpdatesPerSecond, 60 / _JphPhysicsSystemUpdatesPerSecond, &temp_allocator, &job_system_thread_pool);
}

void Arcadia::PhysicsSimulator::Query(Arcadia::Scene& scene, const std::string& name)
{
    _AssertFrameNotInBuild();

    const auto& entity_info = scene.GetEntityInfo(name);

    // For now, only actor entity has physics component
    if(entity_info.Type != "actor")
    {
        return;
    }

    auto [physics_comp, transform_comp] = scene.Get<Arcadia::PhysicsComponent, Arcadia::TransformComponent>(name);

    if(physics_comp.HasBodyInfo())
    {
        const auto& [uuid, jph_body_info_initial] = physics_comp.GetIdentifiableJphBodyInfo();
        ARCADIA_ASSERT(_JphBodyIdStorage.contains(uuid));
        const auto& jph_body_interface = _JphPhysicsSystem->GetBodyInterface();
        const auto& body_id = _JphBodyIdStorage.at(uuid);

        auto& jph_body_state = physics_comp.JphBodyState;
        jph_body_state.Active = jph_body_interface.IsActive(body_id);
        jph_body_state.LinearVelocity = Arcadia::FromJphVec3(jph_body_interface.GetLinearVelocity(body_id));
        jph_body_state.AngularVelocity = Arcadia::FromJphVec3(jph_body_interface.GetAngularVelocity(body_id));

        transform_comp.Position = Arcadia::FromJphVec3(jph_body_interface.GetPosition(body_id));
        transform_comp.Rotation = Arcadia::FromJphQuat(jph_body_interface.GetRotation(body_id));
    }
}

void Arcadia::PhysicsSimulator::Reset()
{
    auto& jph_body_interface = _JphPhysicsSystem->GetBodyInterface();
    for(const auto& [uuid, body_id] : _JphBodyIdStorage)
    {
        jph_body_interface.RemoveBody(body_id);
        jph_body_interface.DestroyBody(body_id);
    }
    _JphBodyIdStorage.clear();
    _SubmittedBodyInfos.clear();
}

auto Arcadia::PhysicsSimulator::ShouldUpdate() const -> bool
{
    return _ShouldUpdate;
}

void Arcadia::PhysicsSimulator::ShouldUpdate(bool should_update)
{
    _ShouldUpdate = should_update;
}

auto Arcadia::PhysicsSimulator::GetJphTempAllocatorSize() const -> JPH::uint
{
    return _JphTempAllocatorSize;
}

void Arcadia::PhysicsSimulator::SetJphTempAllocatorSize(JPH::uint jph_temp_allocator_size)
{
    _JphTempAllocatorSize = jph_temp_allocator_size;
}

auto Arcadia::PhysicsSimulator::GetJphPhysicsSystemUpdatesPerSecond() const -> int
{
    return _JphPhysicsSystemUpdatesPerSecond;
}

void Arcadia::PhysicsSimulator::SetJphPhysicsSystemUpdatesPerSecond(int jph_physics_system_updates_per_second)
{
    _JphPhysicsSystemUpdatesPerSecond = jph_physics_system_updates_per_second;
}

auto Arcadia::PhysicsSimulator::GetJphBodyIdStorage() const -> const jph_body_id_storage_type&
{
    return _JphBodyIdStorage;
}

void Arcadia::PhysicsSimulator::_AssertFrameInBuild() const
{
    ARCADIA_ASSERT(_InBuild && "Frame is not in build, did you call `prepare()`?");
}

void Arcadia::PhysicsSimulator::_AssertFrameNotInBuild() const
{
    ARCADIA_ASSERT(!_InBuild && "Frame is in build, did you call `finalize()`?");
}

auto Arcadia::JphObjectLayerPairFilerImpl::ShouldCollide(JPH::ObjectLayer obj_1, JPH::ObjectLayer obj_2) const -> bool
{
    switch(obj_1)
    {
        case Arcadia::JphObjectLayers::NonMoving:
        {
            return obj_2 == Arcadia::JphObjectLayers::Moving; // Non moving only collides with moving
        }
        case Arcadia::JphObjectLayers::Moving:
        {
            return true; // Moving collides with everything
        }
        default:
        {
            return false;
        }
    }
}

Arcadia::JphBroadPhaseLayerImpl::JphBroadPhaseLayerImpl()
{
    _ObjectToBroadPhase[Arcadia::JphObjectLayers::NonMoving] = Arcadia::JphBroadPhaseLayers::NonMoving;
    _ObjectToBroadPhase[Arcadia::JphObjectLayers::Moving] = Arcadia::JphBroadPhaseLayers::Moving;
}

auto Arcadia::JphBroadPhaseLayerImpl::GetNumBroadPhaseLayers() const -> JPH::uint
{
    return Arcadia::JphBroadPhaseLayers::NumLayers;
}

auto Arcadia::JphBroadPhaseLayerImpl::GetBroadPhaseLayer(JPH::ObjectLayer layer) const -> JPH::BroadPhaseLayer
{
    ARCADIA_ASSERT(layer < GetNumBroadPhaseLayers());
    return _ObjectToBroadPhase[layer];
}

auto Arcadia::JphObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(JPH::ObjectLayer obj, JPH::BroadPhaseLayer bp) const -> bool
{
    switch(obj)
    {
        case Arcadia::JphObjectLayers::NonMoving:
        {
            return bp == Arcadia::JphBroadPhaseLayers::Moving;
        }
        case Arcadia::JphObjectLayers::Moving:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}
