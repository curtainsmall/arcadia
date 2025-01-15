#include"pch.hpp"

#include "glm_algorithm.hpp"

#include"core/assert.hpp"

void Arcadia::Glm::Decompose(
    const glm::mat4& transform,
    glm::vec3& translation,
    glm::quat& rotation,
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
        glm::epsilonNotEqual(local_matrix[2][3], static_cast<T>(0), glm::epsilon<T>()))
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
        for(glm::length_t j = 0; j < 3; ++j)
            row[i][j] = local_matrix[i][j];

    // Compute X scale factor and normalize first row.
    scale.x = glm::length(row[0]);
    row[0] = glm::detail::scale(row[0], static_cast<T>(1));
    scale.y = glm::length(row[1]);
    row[1] = glm::detail::scale(row[1], static_cast<T>(1));
    scale.z = glm::length(row[2]);
    row[2] = glm::detail::scale(row[2], static_cast<T>(1));

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

    int i, j, k = 0;
    T root, trace = row[0].x + row[1].y + row[2].z;
    if(trace > static_cast<T>(0))
    {
        root = sqrt(trace + static_cast<T>(1.0));
        rotation.w = static_cast<T>(0.5) * root;
        root = static_cast<T>(0.5) / root;
        rotation.x = root * (row[1].z - row[2].y);
        rotation.y = root * (row[2].x - row[0].z);
        rotation.z = root * (row[0].y - row[1].x);
    } // End if > 0
    else
    {
        static int Next[3] = { 1, 2, 0 };
        i = 0;
        if(row[1].y > row[0].x) i = 1;
        if(row[2].z > row[i][i]) i = 2;
        j = Next[i];
        k = Next[j];

    #           ifdef GLM_FORCE_QUAT_DATA_WXYZ
        int off = 1;
    #           else
        int off = 0;
    #           endif

        root = sqrt(row[i][i] - row[j][j] - row[k][k] + static_cast<T>(1.0));

        rotation[i + off] = static_cast<T>(0.5) * root;
        root = static_cast<T>(0.5) / root;
        rotation[j + off] = root * (row[i][j] + row[j][i]);
        rotation[k + off] = root * (row[i][k] + row[k][i]);
        rotation.w = root * (row[j][k] - row[k][j]);
    } // End if <= 0
}