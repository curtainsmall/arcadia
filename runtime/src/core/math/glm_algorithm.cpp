#include "pch.hpp"

#include "glm_algorithm.hpp"

#include "core/assert.hpp"

void Arcadia::Glm::Decompose(
    const glm::mat4& transform,
    glm::vec3& translation,
    glm::vec3& rotation,
    glm::vec3& scale
)
{
    // From glm::decompose in matrix_decompose.inl

    using T = float;

    glm::mat4 local_matrix(transform);

    // Normalize the matrix.
    ACDA_ASSERT(!glm::epsilonEqual(local_matrix[3][3], static_cast<T>(0), glm::epsilon<T>()));

    // First, isolate perspective.  This is the messiest.
    if(
        glm::epsilonNotEqual(local_matrix[0][3], static_cast<T>(0), glm::epsilon<T>()) ||
        glm::epsilonNotEqual(local_matrix[1][3], static_cast<T>(0), glm::epsilon<T>()) ||
        glm::epsilonNotEqual(local_matrix[2][3], static_cast<T>(0), glm::epsilon<T>())
        )
    {
        // Clear the perspective partition
        local_matrix[0][3] = local_matrix[1][3] = local_matrix[2][3] = static_cast<T>(0);
        local_matrix[3][3] = static_cast<T>(1);
    }

    // Next take care of translation (easy).
    translation = glm::vec3(local_matrix[3]);
    local_matrix[3] = glm::vec4(0, 0, 0, local_matrix[3].w);

    glm::vec3 row[3], pdum3;

    // Now get scale and shear.
    for(glm::length_t i = 0; i < 3; ++i)
    {
        for(glm::length_t j = 0; j < 3; ++j)
        {
            row[i][j] = local_matrix[i][j];
        }
    }

    // Compute X scale factor and normalize first row.
    scale.x = glm::length(row[0]);
    row[0] = glm::detail::scale(row[0], static_cast<T>(1));
    scale.y = glm::length(row[1]);
    row[1] = glm::detail::scale(row[1], static_cast<T>(1));
    scale.z = glm::length(row[2]);
    row[2] = glm::detail::scale(row[2], static_cast<T>(1));

    rotation.y = asin(-row[0][2]);
    if(cos(rotation.y) != 0)
    {
        rotation.x = atan2(row[1][2], row[2][2]);
        rotation.z = atan2(row[0][1], row[0][0]);
    }
    else
    {
        rotation.x = atan2(-row[2][0], row[1][1]);
        rotation.z = 0;
    }
}