#pragma once

#define JPH_FLOATING_POINT_EXCEPTIONS_ENABLED
#define JPH_PROFILE_ENABLED
#define JPH_DEBUG_RENDERER

#include"Jolt/Jolt.h"

#include"Jolt/Core/Factory.h"
#include"Jolt/Core/JobSystemThreadPool.h"
#include"Jolt/Physics/Body/BodyCreationSettings.h"
#include"Jolt/Physics/Body/MotionType.h"
#include"Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h"
#include"Jolt/Physics/Collision/ObjectLayer.h"
#include"Jolt/Physics/Collision/Shape/BoxShape.h"
#include"Jolt/Physics/Collision/Shape/CapsuleShape.h"
#include"Jolt/Physics/Collision/Shape/CylinderShape.h"
#include"Jolt/Physics/Collision/Shape/SphereShape.h"
#include"Jolt/Physics/PhysicsSettings.h"
#include"Jolt/Physics/PhysicsSystem.h"
#include"Jolt/RegisterTypes.h"

#include"core/math.hpp"

namespace Arcadia
{
    [[nodiscard]]
    static inline auto ToJphVec3(const glm::vec3& vec) -> JPH::Vec3
    {
        return {
            vec.x,
            vec.y,
            vec.z
        };
    }
    [[nodiscard]]
    static inline auto FromJphVec3(const JPH::Vec3& jolt_vec) -> glm::vec3
    {
        return {
            jolt_vec.GetX(),
            jolt_vec.GetY(),
            jolt_vec.GetZ()
        };
    }

    [[nodiscard]]
    static inline auto ToJphVec4(const glm::vec4& vec) -> JPH::Vec4
    {
        return {
            vec.x,
            vec.y,
            vec.z,
            vec.w
        };
    }
    [[nodiscard]]
    static inline auto FromJphVec4(const JPH::Vec4& jolt_vec) -> glm::vec4
    {
        return {
            jolt_vec.GetX(),
            jolt_vec.GetY(),
            jolt_vec.GetZ(),
            jolt_vec.GetW()
        };
    }

    [[nodiscard]]
    static inline auto ToJphQuat(const glm::quat& quat) -> JPH::Quat
    {
        return JPH::Quat{
            quat.x,
            quat.y,
            quat.z,
            quat.w
        };
    }
    [[nodiscard]]
    static inline auto FromJphQuat(const JPH::Quat& jolt_quat) -> glm::quat
    {
        return {
            jolt_quat.GetW(),
            jolt_quat.GetX(),
            jolt_quat.GetY(),
            jolt_quat.GetZ()
        };
    }

    namespace JphObjectLayers
    {
        static constexpr JPH::ObjectLayer NonMoving{ 0 };
        static constexpr JPH::ObjectLayer Moving{ 1 };
        static constexpr size_t NumLayers{ 2 };
    }

    namespace JphBroadPhaseLayers
    {
        static constexpr JPH::BroadPhaseLayer NonMoving{ 0 };
        static constexpr JPH::BroadPhaseLayer Moving{ 0 };
        static constexpr size_t NumLayers{ 2 };
    }
}