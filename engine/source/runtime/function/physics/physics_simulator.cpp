#include "physics_simulator.hpp"

#include <thread>

#include "core/match.hpp"
#include "resource/scene_layer.hpp"

Arcadia::PhysicsSimulator::PhysicsSimulator()
{
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    const JPH::uint max_bodies = 65536;
    const JPH::uint num_body_mutexes = 0;
    const JPH::uint max_body_pair = 65535;
    const JPH::uint max_contact_constraints = 10240;

    _JphPhysicsSystem.Init(max_bodies, num_body_mutexes, max_body_pair, max_contact_constraints, _JphBroadPhaseLayer, _JphObjectVsBroadLayerFilter, _JphObjectLayerPairFilter);
    _JphJobSystemThreadPool.Init(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, static_cast<int>(std::thread::hardware_concurrency() - 1)
    );
}

Arcadia::PhysicsSimulator::~PhysicsSimulator()
{
    JPH::UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}

auto Arcadia::PhysicsSimulator::HasEntity(EntityId entity_id) const -> bool
{
    return _JphBodyIdStorage.contains(entity_id);
}

void Arcadia::PhysicsSimulator::BuildEntity(EntityId entity_id)
{
    _EntityIdSet.emplace(entity_id);

    std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();
    const EntityInfo& entity_info = scene_layer_sptr->ActiveScene_GetEntityInfo(entity_id);

    if(entity_info.Type != EntityType::Actor)
    {
        return;
    }

    const auto& [physics_comp, transform_comp] = scene_layer_sptr->ActiveScene_GetComponent<PhysicsComponent, TransformComponent>(entity_id);

    if(physics_comp.IsValid())
    {
        JPH::BodyInterface& jph_body_interface = _JphPhysicsSystem.GetBodyInterface();

        if(HasEntity(entity_id))
        {
            _UpdateJphBody(jph_body_interface, physics_comp, transform_comp, entity_id);
        }
        else
        {
            _CreateJphBody(jph_body_interface, physics_comp, transform_comp, entity_id);
        }
    }
}

void Arcadia::PhysicsSimulator::RemoveEntity(EntityId entity_id)
{
    if(!HasEntity(entity_id))
    {
        return;
    }
    JPH::BodyInterface& jph_body_interface = _JphPhysicsSystem.GetBodyInterface();
    JPH::BodyID body_id = _JphBodyIdStorage.at(entity_id);
    jph_body_interface.RemoveBody(body_id);
    jph_body_interface.DestroyBody(body_id);
    _JphBodyIdStorage.erase(entity_id);
    _EntityIdSet.erase(entity_id);
}

void Arcadia::PhysicsSimulator::Update()
{
    // Update physics simulation
    JPH::TempAllocatorImpl temp_allocator(_JphTempAllocatorSize);

    int collusion_step = 60 / _JphPhysicsSystemUpdatesPerSecond;
    collusion_step = collusion_step > 0 ? collusion_step : 1;
    _JphPhysicsSystem.Update(
        1.f / _JphPhysicsSystemUpdatesPerSecond,
        collusion_step,
        &temp_allocator,
        &_JphJobSystemThreadPool
    );
}

void Arcadia::PhysicsSimulator::ApplyToEntites()
{
    for(const EntityId& entity_id : _EntityIdSet)
    {
        std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();
        const EntityInfo& entity_info = scene_layer_sptr->ActiveScene_GetEntityInfo(entity_id);

        if(entity_info.Type != EntityType::Actor)
        {
            continue;
        }

        auto [physics_comp, transform_comp] = scene_layer_sptr->ActiveScene_GetComponent<PhysicsComponent, TransformComponent>(entity_id);

        if(physics_comp.IsValid())
        {
            const JPH::BodyInterface& jph_body_interface = _JphPhysicsSystem.GetBodyInterface();
            const JPH::BodyID& body_id = _JphBodyIdStorage.at(entity_id);

            physics_comp.SetActive(jph_body_interface.IsActive(body_id));
            physics_comp.SetLinearVelocity(FromJphVec3(jph_body_interface.GetLinearVelocity(body_id)));
            physics_comp.SetAngularVelocity(FromJphVec3(jph_body_interface.GetAngularVelocity(body_id)));

            transform_comp.SetPosition(FromJphVec3(jph_body_interface.GetPosition(body_id)));
            transform_comp.SetRotationQuaternion(FromJphQuat(jph_body_interface.GetRotation(body_id)));
        }
    }
}

void Arcadia::PhysicsSimulator::Reset()
{
    JPH::BodyInterface& jph_body_interface = _JphPhysicsSystem.GetBodyInterface();
    for(const auto& [uuid, body_id] : _JphBodyIdStorage)
    {
        jph_body_interface.RemoveBody(body_id);
        jph_body_interface.DestroyBody(body_id);
    }
    _JphBodyIdStorage.clear();
    _EntityIdSet.clear();
}

auto Arcadia::PhysicsSimulator::IsActive() const -> bool
{
    return _Active;
}

void Arcadia::PhysicsSimulator::SetActive(bool active)
{
    _Active = active;
}

auto Arcadia::PhysicsSimulator::GetJphTempAllocatorSize() const -> JPH::uint
{
    return _JphTempAllocatorSize;
}

void Arcadia::PhysicsSimulator::SetJphTempAllocatorSize(JPH::uint jph_temp_allocator_size)
{
    _JphTempAllocatorSize = jph_temp_allocator_size;
}

auto Arcadia::PhysicsSimulator::GetJphPhysicsSystemUpdatesPerSecond() const -> std::int32_t
{
    return _JphPhysicsSystemUpdatesPerSecond;
}

void Arcadia::PhysicsSimulator::SetJphPhysicsSystemUpdatesPerSecond(std::int32_t jph_physics_system_updates_per_second)
{
    _JphPhysicsSystemUpdatesPerSecond = jph_physics_system_updates_per_second;
}

auto Arcadia::PhysicsSimulator::GetBodyCount() const->std::size_t
{
    return _JphBodyIdStorage.size();
}

void Arcadia::PhysicsSimulator::_Clear()
{
    _JphBodyIdStorage.clear();
}

auto Arcadia::PhysicsSimulator::_CreateJphShape(const PhysicsComponent& physics_comp) const -> const JPH::Shape*
{
    return MatchVariant<JPH::Shape*>(
        physics_comp.GetJphShapeInfo(),
        [&](const JphNullShapeInfo&)
        {
            ACDA_UNREACHABLE("Invalid shape info type");
            return nullptr;
        },
        [&](const JphBoxShapeInfo& info)
        {
            return new JPH::BoxShape(ToJphVec3(info.HalfExtent), info.ConvexRadius);
        },
        [&](const JphCapsuleShapeInfo& info)
        {
            return new JPH::CapsuleShape(info.HalfHeightOfCylinder, info.Radius);
        },
        [&](const JphCylinderShapeInfo& info)
        {
            return new JPH::CylinderShape(info.HalfHeight, info.Radius, info.ConvexRadius);
        },
        [&](const JphSphereShapeInfo& info)
        {
            return new JPH::SphereShape(info.Radius);
        }
    );
}

void Arcadia::PhysicsSimulator::_CreateJphBody(
    JPH::BodyInterface& jph_body_interface,
    const PhysicsComponent& physics_comp,
    const TransformComponent& transform_comp,
    EntityId entity_id
)
{
    JPH::ShapeRefC jph_shape_refc = _CreateJphShape(physics_comp);
    const JPH::BodyID body_id = jph_body_interface.CreateAndAddBody(
        JPH::BodyCreationSettings(
            jph_shape_refc,
            ToJphVec3(transform_comp.GetPosition()),
            ToJphQuat(transform_comp.GetRotationQuaternion()),
            physics_comp.GetJphMotionType(),
            physics_comp.GetJphObjectLayer()
        ),
        JPH::EActivation::Activate
    );
    ACDA_ASSERT(!body_id.IsInvalid() && "Failed to create body");
    _JphBodyIdStorage.try_emplace(entity_id, body_id);
}

void Arcadia::PhysicsSimulator::_UpdateJphBody(
    JPH::BodyInterface& jph_body_interface,
    const PhysicsComponent& physics_comp,
    const TransformComponent& transform_comp,
    EntityId entity_id)
{
    const JPH::BodyID body_id = _JphBodyIdStorage.at(entity_id);
    const JPH::Shape* jph_shape_ptr = _CreateJphShape(physics_comp);

    jph_body_interface.SetShape(body_id, jph_shape_ptr, true, JPH::EActivation::DontActivate);
    jph_body_interface.SetPositionAndRotation(
        body_id,
        ToJphVec3(transform_comp.GetPosition()),
        ToJphQuat(transform_comp.GetRotationQuaternion()),
        JPH::EActivation::DontActivate
    );
    jph_body_interface.SetLinearVelocity(body_id, ToJphVec3(physics_comp.GetLinearVelocity()));
    jph_body_interface.SetMotionType(body_id, physics_comp.GetJphMotionType(), JPH::EActivation::DontActivate);
    jph_body_interface.SetObjectLayer(body_id, physics_comp.GetJphObjectLayer());
}

auto Arcadia::JphObjectLayerPairFilerImpl::ShouldCollide(JPH::ObjectLayer obj_1, JPH::ObjectLayer obj_2) const -> bool
{
    switch(obj_1)
    {
        case JphObjectLayers::NonMoving:
        {
            return obj_2 == JphObjectLayers::Moving; // Non moving only collides with moving
        }
        case JphObjectLayers::Moving:
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
    _ObjectToBroadPhase[JphObjectLayers::NonMoving] = JphBroadPhaseLayers::NonMoving;
    _ObjectToBroadPhase[JphObjectLayers::Moving] = JphBroadPhaseLayers::Moving;
}

auto Arcadia::JphBroadPhaseLayerImpl::GetNumBroadPhaseLayers() const -> JPH::uint
{
    return JphBroadPhaseLayers::NumLayers;
}

auto Arcadia::JphBroadPhaseLayerImpl::GetBroadPhaseLayer(JPH::ObjectLayer layer) const -> JPH::BroadPhaseLayer
{
    ACDA_ASSERT(layer < GetNumBroadPhaseLayers());
    return _ObjectToBroadPhase[layer];
}

auto Arcadia::JphObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(JPH::ObjectLayer obj, JPH::BroadPhaseLayer bp) const -> bool
{
    switch(obj)
    {
        case JphObjectLayers::NonMoving:
        {
            return bp == JphBroadPhaseLayers::Moving;
        }
        case JphObjectLayers::Moving:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}