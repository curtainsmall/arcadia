#pragma once

#include<optional>
#include<tuple>

#include"core/math/glm_mat4.hpp"
#include"core/math/glm_vec3.hpp"

namespace Arcadia
{
    auto Decompose(const glm::mat4& transform)
        -> std::tuple<
        glm::vec3, // Translation
        glm::vec3, // Rotation
        glm::vec3  // Scale
        >;
}