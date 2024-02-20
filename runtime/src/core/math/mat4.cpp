#include "pch.hpp"
#include "mat4.hpp"

#include"core/math/vec3.hpp"

ARCADIA_API auto arcadia::detail::_transform_with_pivot_to_orignal_and_back(const glm::vec3& pivot, const glm::mat4& transform) -> glm::mat4
{
    auto pivot_to_origin_mat4 = glm::translate(arcadia::mat4::create_identity(), -pivot);
    auto origin_to_pivot_mat4 = glm::translate(arcadia::mat4::create_identity(), pivot);

    return origin_to_pivot_mat4 * transform * pivot_to_origin_mat4;
}

ARCADIA_API auto arcadia::gen_scaling_mat4(const glm::vec3& scale, const glm::vec3& pivot) -> glm::mat4
{
    auto transform_mat4 = glm::scale(arcadia::mat4::create_identity(), scale);
    return arcadia::detail::_transform_with_pivot_to_orignal_and_back(pivot, transform_mat4);
}

ARCADIA_API auto arcadia::gen_rotation_mat4(const glm::vec3& rotation, const glm::vec3& pivot) -> glm::mat4
{
    auto transform_mat4 = arcadia::mat4::create_identity();

    transform_mat4 = glm::rotate(transform_mat4, rotation.x, arcadia::vec3::create_pos_unit_x());
    transform_mat4 = glm::rotate(transform_mat4, rotation.y, arcadia::vec3::create_pos_unit_y());
    transform_mat4 = glm::rotate(transform_mat4, rotation.z, arcadia::vec3::create_pos_unit_z());

    return arcadia::detail::_transform_with_pivot_to_orignal_and_back(pivot, transform_mat4);
}

ARCADIA_API auto arcadia::gen_position_mat4(const glm::vec3& position, const glm::vec3& pivot) -> glm::mat4
{
    return glm::translate(arcadia::mat4::create_identity(), position - pivot);
}
