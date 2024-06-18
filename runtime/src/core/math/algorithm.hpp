#pragma once

#include<optional>
#include<tuple>

#include"core/math/mat4.hpp"
#include"core/math/vec3.hpp"

/// @brief Decompse transform matrix to translation, rotation and scale components
/// @param transform Transform matrix
/// @return Decomposed components, or nullopt if the transform matrix is zero
auto Decompose(const glm::mat4& transform)
-> std::optional<std::tuple<
    glm::vec3, // Translation
    glm::vec3, // Rotation
    glm::vec3  // Scale
    >>;
