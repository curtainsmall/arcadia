#include"pch.hpp"

#include "glm_algorithm.hpp"

auto Arcadia::Decompose(const glm::mat4& transform) -> std::tuple<glm::vec3, glm::vec3, glm::vec3>
{
    // From glm::decompose in matrix_decompose.inl

    glm::vec3
        translation{},
        rotation{},
        scale{};

    using T = float;

    glm::mat4 local_matrix(transform);

    // Normalize the matrix.
    if(glm::epsilonEqual(local_matrix[3][3], static_cast<T>(0), glm::epsilon<T>()))
    {
        throw Exceptions::NullReturn();
    }

    // First, isolate perspective.  This is the messiest.
    if(
        glm::epsilonNotEqual(local_matrix[0][3], static_cast<T>(0), glm::epsilon<T>()) ||
        glm::epsilonNotEqual(local_matrix[1][3], static_cast<T>(0), glm::epsilon<T>()) ||
        glm::epsilonNotEqual(local_matrix[2][3], static_cast<T>(0), glm::epsilon<T>()))
    {
        // Clear the perspective partition
        local_matrix[0][3] = local_matrix[1][3] = local_matrix[2][3] = static_cast<T>(0);
        local_matrix[3][3] = static_cast<T>(1);
    }

    // Next take care of translation (easy).
    translation = glm::vec3(local_matrix[3]);
    local_matrix[3] = glm::vec4(0, 0, 0, local_matrix[3].w);

    glm::vec3 Row[3], Pdum3;

    // Now get scale and shear.
    for(glm::length_t i = 0; i < 3; ++i)
        for(glm::length_t j = 0; j < 3; ++j)
            Row[i][j] = local_matrix[i][j];

    // Compute X scale factor and normalize first row.
    scale.x = glm::length(Row[0]);
    Row[0] = glm::detail::scale(Row[0], static_cast<T>(1));
    scale.y = glm::length(Row[1]);
    Row[1] = glm::detail::scale(Row[1], static_cast<T>(1));
    scale.z = glm::length(Row[2]);
    Row[2] = glm::detail::scale(Row[2], static_cast<T>(1));

    // At this point, the matrix (in rows[]) is orthonormal.
    // Check for a coordinate system flip.  If the determinant
    // is -1, then negate the matrix and the scaling factors.
#if 0
    Pdum3 = glm::cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
    if(glm::dot(Row[0], Pdum3) < 0)
    {
        for(glm::length_t i = 0; i < 3; i++)
        {
            Scale[i] *= static_cast<T>(-1);
            Row[i] *= static_cast<T>(-1);
        }
    }
#endif

    rotation.y = asin(-Row[0][2]);
    if(cos(rotation.y) != 0)
    {
        rotation.x = atan2(Row[1][2], Row[2][2]);
        rotation.z = atan2(Row[0][1], Row[0][0]);
    }
    else
    {
        rotation.x = atan2(-Row[2][0], Row[1][1]);
        rotation.z = 0;
    }

    return std::make_tuple(translation, rotation, scale);
}