#pragma once

#include"Jolt/Jolt.h"

#include"core/math.hpp"

namespace arcadia
{
    [[nodiscard]]
    inline auto to_jolt_vec3(const glm::vec3& vec) -> JPH::Vec3
    {
        return {
            vec.x,
            vec.y,
            vec.z
        };
    }
    [[nodiscard]]
    inline auto from_jolt_vec3(const JPH::Vec3& jolt_vec) -> glm::vec3
    {
        return {
            jolt_vec.GetX(),
            jolt_vec.GetY(),
            jolt_vec.GetZ()
        };
    }

    [[nodiscard]]
    inline auto to_jolt_vec4(const glm::vec4& vec) -> JPH::Vec4
    {
        return {
            vec.x,
            vec.y,
            vec.z,
            vec.w
        };
    }
    [[nodiscard]]
    inline auto from_jolt_vec4(const JPH::Vec4& jolt_vec) -> glm::vec4
    {
        return {
            jolt_vec.GetX(),
            jolt_vec.GetY(),
            jolt_vec.GetZ(),
            jolt_vec.GetW()
        };
    }

    [[nodiscard]]
    inline auto to_jolt_quat(const glm::quat& quat) -> JPH::Quat
    {
        return {
            quat.w,
            quat.x,
            quat.y,
            quat.z
        };
    }
    [[nodiscard]]
    inline auto from_jolt_quat(const JPH::Quat& jolt_quat) -> glm::quat
    {
        return {
            jolt_quat.GetW(),
            jolt_quat.GetX(),
            jolt_quat.GetY(),
            jolt_quat.GetZ()
        };
    }
}