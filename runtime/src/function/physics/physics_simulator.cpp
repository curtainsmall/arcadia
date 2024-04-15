#include "pch.hpp"
#include "physics_simulator.hpp"

#include<thread>

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

void Arcadia::PhysicsSimulator::Submit(const Arcadia::PhysicsComponent& physics_comp)
{
    _AssertFrameInBuild();

    if(physics_comp.HasBodyInfo())
    {
        const auto& [Uuid, body_info] = physics_comp.GetIdentifiableJphBodyInfoInitial();
        if(!_JphBodyIdStorage.contains(Uuid))
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
            auto body_id = jph_body_interface.CreateAndAddBody(
                JPH::BodyCreationSettings{
                    jph_shape_refc,
                    Arcadia::ToJphVec3(body_info.Position),
                    Arcadia::ToJphQuat(body_info.Rotation),
                    body_info.JphMotionType,
                    body_info.JphObjectLayer
                },
                JPH::EActivation::Activate
            );
            if(body_id.IsInvalid())
            {
                throw SubmitFail{ std::format("Failed to create body; Its uuid is {}",Uuid) };
            }
            _JphBodyIdStorage.try_emplace(Uuid, body_id);
        }
        _SubmittedBodyInfos.emplace(Uuid);
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

void Arcadia::PhysicsSimulator::Quary(PhysicsComponent& physics_comp)
{
    _AssertFrameNotInBuild();

    if(physics_comp.HasBodyInfo())
    {
        const auto& [Uuid, jph_body_info_initial] = physics_comp.GetIdentifiableJphBodyInfoInitial();
        if(_JphBodyIdStorage.contains(Uuid))
        {
            const auto& jph_body_interface = _JphPhysicsSystem->GetBodyInterface();
            const auto& body_id = _JphBodyIdStorage.at(Uuid);

            auto& jph_body_info_ongoing = physics_comp.GetJphBodyInfoOngoing();
            jph_body_info_ongoing.Active = jph_body_interface.IsActive(body_id);
            jph_body_info_ongoing.Position = Arcadia::FromJphVec3(jph_body_interface.GetPosition(body_id));
            jph_body_info_ongoing.Rotation = Arcadia::FromJphQuat(jph_body_interface.GetRotation(body_id));
            jph_body_info_ongoing.LinearVelocity = Arcadia::FromJphVec3(jph_body_interface.GetLinearVelocity(body_id));
            jph_body_info_ongoing.AngularVelocity = Arcadia::FromJphVec3(jph_body_interface.GetAngularVelocity(body_id));
        }
        else
        {
            throw UnknownPhysicsComponent{ "Cannot quary component that has not been submitted" };
        }
    }
}

void Arcadia::PhysicsSimulator::Reset()
{
    auto& jph_body_interface = _JphPhysicsSystem->GetBodyInterface();
    for(const auto& [Uuid, body_id] : _JphBodyIdStorage)
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

auto Arcadia::PhysicsSimulator::GetJphBodyIdUmap() const -> const jph_body_id_storage_type&
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
