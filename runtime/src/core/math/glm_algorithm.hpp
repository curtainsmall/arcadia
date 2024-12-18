#pragma once

#include<optional>
#include<tuple>

#include"core/math/glm_mat4.hpp"
#include"core/math/glm_vec3.hpp"

namespace Arcadia
{
    /// @brief Decompse transform matrix to translation, rotation and scale components
    /// @param transform Transform matrix
    /// @return Decomposed components, or nullopt if the transform matrix is zero
    auto Decompose(const glm::mat4& transform)
        -> std::tuple<
        glm::vec3, // Translation
        glm::vec3, // Rotation
        glm::vec3  // Scale
        >;
}