#pragma once

#include"core/base.hpp"

#ifdef ARCADIA_IN_DEBUG
#define JPH_FLOATING_POINT_EXCEPTIONS_ENABLED
#define JPH_PROFILE_ENABLED
#define JPH_DEBUG_RENDERER
#endif

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
#include"Jolt/Physics/PhysicsSystem.h"
#include"Jolt/Physics/PhysicsSettings.h"
#include"Jolt/RegisterTypes.h"

#include"core/math.hpp"

namespace arcadia
{
    [[nodiscard]]
    inline auto to_jph_vec3(const glm::vec3& vec) -> JPH::Vec3
    {
        return {
            vec.x,
            vec.y,
            vec.z
        };
    }
    [[nodiscard]]
    inline auto from_jph_vec3(const JPH::Vec3& jolt_vec) -> glm::vec3
    {
        return {
            jolt_vec.GetX(),
            jolt_vec.GetY(),
            jolt_vec.GetZ()
        };
    }

    [[nodiscard]]
    inline auto to_jph_vec4(const glm::vec4& vec) -> JPH::Vec4
    {
        return {
            vec.x,
            vec.y,
            vec.z,
            vec.w
        };
    }
    [[nodiscard]]
    inline auto from_jph_vec4(const JPH::Vec4& jolt_vec) -> glm::vec4
    {
        return {
            jolt_vec.GetX(),
            jolt_vec.GetY(),
            jolt_vec.GetZ(),
            jolt_vec.GetW()
        };
    }

    [[nodiscard]]
    inline auto to_jph_quat(const glm::quat& quat) -> JPH::Quat
    {
        return JPH::Quat{
            quat.x,
            quat.y,
            quat.z,
            quat.w
        };
    }
    [[nodiscard]]
    inline auto from_jph_quat(const JPH::Quat& jolt_quat) -> glm::quat
    {
        return {
            jolt_quat.GetW(),
            jolt_quat.GetX(),
            jolt_quat.GetY(),
            jolt_quat.GetZ()
        };
    }

    namespace jph_object_layers
    {
        static constexpr JPH::ObjectLayer non_moving{ 0 };
        static constexpr JPH::ObjectLayer moving{ 1 };
        static constexpr std::size_t num_layers{ 2 };
    }

    namespace jph_broad_phase_layers
    {
        static constexpr JPH::BroadPhaseLayer non_moving{ 0 };
        static constexpr JPH::BroadPhaseLayer moving{ 0 };
        static constexpr std::size_t num_layers{ 2 };
    }

}

