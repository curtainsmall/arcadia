#pragma once

#include<optional>
#include<tuple>

#include"core/math/glm_mat4.hpp"
#include"core/math/glm_vec3.hpp"

namespace Arcadia
{
    namespace Glm
    {
        void Decompose(
            const glm::mat4& transform,
            glm::vec3& translation,
            glm::vec3& rotation,
            glm::vec3& scale
        );
    }
}