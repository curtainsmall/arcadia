

#include"jolt.hpp"

auto Arcadia::ToJphVec3(const glm::vec3& vec) -> JPH::Vec3
{
    return JPH::Vec3(
        vec.x,
        vec.y,
        vec.z
    );
}

auto Arcadia::FromJphVec3(const JPH::Vec3& jolt_vec) -> glm::vec3
{
    return glm::vec3(
        jolt_vec.GetX(),
        jolt_vec.GetY(),
        jolt_vec.GetZ()
    );
}

auto Arcadia::ToJphVec4(const glm::vec4& vec) -> JPH::Vec4
{
    return JPH::Vec4(
        vec.x,
        vec.y,
        vec.z,
        vec.w
    );
}

auto Arcadia::FromJphVec4(const JPH::Vec4& jolt_vec) -> glm::vec4
{
    return glm::vec4(
        jolt_vec.GetX(),
        jolt_vec.GetY(),
        jolt_vec.GetZ(),
        jolt_vec.GetW()
    );
}

auto Arcadia::ToJphQuat(const glm::quat& quat) -> JPH::Quat
{
    return JPH::Quat(
        quat.x,
        quat.y,
        quat.z,
        quat.w
    );
}

auto Arcadia::FromJphQuat(const JPH::Quat& jolt_quat) -> glm::quat
{
    return glm::qua(
        jolt_quat.GetW(),
        jolt_quat.GetX(),
        jolt_quat.GetY(),
        jolt_quat.GetZ()
    );
}
