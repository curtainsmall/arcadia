#include "pch.hpp"

#include "glm.hpp"

#include "boost/math/special_functions.hpp"

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

auto Arcadia::Glm::Mat2_ToJson(const glm::mat2& mat) -> nlohmann::json
{
    return nlohmann::json::array(
        {
            Glm::Vec2_ToJson(mat[0]),
            Glm::Vec2_ToJson(mat[1])
        }
    );
}

auto Arcadia::Glm::Mat2_FromJson(const nlohmann::json& json) -> glm::mat2
{
    return glm::mat2(
        Glm::Vec2_FromJson(json[0]),
        Glm::Vec2_FromJson(json[1])
    );
}

auto Arcadia::Glm::Mat2_CreateZero() -> glm::mat2
{
    return glm::mat2(.0f);
}

auto Arcadia::Glm::Mat2_CreateIdentity() -> glm::mat2
{
    return glm::mat2{};
}

auto Arcadia::Glm::Mat3_ToJson(const glm::mat3& mat) -> nlohmann::json
{
    return nlohmann::json::array(
        {
            Glm::Vec3_ToJson(mat[0]),
            Glm::Vec3_ToJson(mat[1])
        }
    );
}

auto Arcadia::Glm::Mat3_FromJson(const nlohmann::json& json) -> glm::mat3
{
    return glm::mat3(
        Glm::Vec3_FromJson(json[0]),
        Glm::Vec3_FromJson(json[1]),
        Glm::Vec3_FromJson(json[2])
    );
}

auto Arcadia::Glm::Mat3_CreateZero() -> glm::mat3
{
    return glm::mat3(.0f);
}

auto Arcadia::Glm::Mat3_CreateIdentity() -> glm::mat3
{
    return glm::mat3{};
}

auto Arcadia::Glm::Mat4_ToJson(const glm::mat4& mat) -> nlohmann::json
{
    return nlohmann::json::array(
        {
            Glm::Vec4_ToJson(mat[0]),
            Glm::Vec4_ToJson(mat[1]),
            Glm::Vec4_ToJson(mat[2]),
            Glm::Vec4_ToJson(mat[3])
        }
    );
}

auto Arcadia::Glm::Mat4_FromJson(const nlohmann::json& json) -> glm::mat4
{
    return glm::mat4(
        Glm::Vec4_FromJson(json[0]),
        Glm::Vec4_FromJson(json[1]),
        Glm::Vec4_FromJson(json[2]),
        Glm::Vec4_FromJson(json[3])
    );
}

auto Arcadia::Glm::Mat4_CreateZero() -> glm::mat4
{
    return glm::mat4(.0f);
}

auto Arcadia::Glm::Mat4_CreateZeroAffine() -> glm::mat4
{
    return glm::mat4(
        glm::vec4{},
        glm::vec4{},
        glm::vec4{},
        glm::vec4{ .0f,.0f,.0f,1.f }
    );
}

auto Arcadia::Glm::Mat4_CreateIdentity() -> glm::mat4
{
    return glm::mat4{};
}

auto Arcadia::Glm::Quat_ToJson(const glm::quat& quat) -> nlohmann::json
{
    return nlohmann::json::array(
        {
            quat.w,
            quat.x,
            quat.y,
            quat.z
        }
    );
}

auto Arcadia::Glm::Quat_FromJson(const nlohmann::json& json) -> glm::quat
{
    return glm::quat(
        json[0],
        json[1],
        json[2],
        json[3]
    );
}

auto Arcadia::Glm::Quat_CreateZero() -> glm::quat
{
    return glm::quat{};
}

auto Arcadia::Glm::Quat_CreateIdentity() -> glm::quat
{
    return glm::quat(1.f, .0f, .0f, .0f);
}

auto Arcadia::Glm::Quat_NormalizeFixedly(const glm::quat& quat, glm::quat::length_type index) -> glm::quat
{
    ACDA_ASSERT(index >= 0 && index < 4);

    auto fixed = quat[index];
    auto a = quat[(index + 1) % 4];
    auto b = quat[(index + 2) % 4];
    auto c = quat[(index + 3) % 4];

    auto R = std::sqrt(1 - fixed * fixed); // Radius of target cicle
    auto r = std::sqrt(a * a + b * b + c * c); // Radius of the circle point (a,b,c) is on
    if(r != 0)
    {
        a = a * R / r;
        b = b * R / r;
        c = c * R / r;
    }
    else
    {
        a = R;
        b = 0;
        c = 0;
    }

    glm::quat res{};
    res[index] = fixed;
    res[(index + 1) % 4] = a;
    res[(index + 2) % 4] = b;
    res[(index + 3) % 4] = c;
    return res;
}

auto Arcadia::Glm::Quat_NormalizeFixedly(const glm::quat& compare, const glm::quat& quat) -> glm::quat
{
    if(compare.w != quat.w)
    {
        return Quat_NormalizeFixedly(quat, 0);
    }
    else if(compare.x != quat.x)
    {
        return Quat_NormalizeFixedly(quat, 1);
    }
    else if(compare.y != quat.y)
    {
        return Quat_NormalizeFixedly(quat, 2);
    }
    else if(compare.z != quat.x)
    {
        return Quat_NormalizeFixedly(quat, 3);
    }
    return quat;
}

auto Arcadia::Glm::Vec2_ToJson(const glm::vec2& vec) -> nlohmann::json
{
    return nlohmann::json::array(
        {
            vec.x,
            vec.y
        }
    );
}

auto Arcadia::Glm::Vec2_FromJson(const nlohmann::json& json) -> glm::vec2
{
    return glm::vec2(
        json[0],
        json[1]
    );
}

auto Arcadia::Glm::Vec2_CreateZero() -> glm::vec2
{
    return glm::vec2{};
}

auto Arcadia::Glm::Vec2_CreateUnitPositiveX() -> glm::vec2
{
    return glm::vec2(1.f, .0f);
}

auto Arcadia::Glm::Vec2_CreateUnitPositiveY() -> glm::vec2
{
    return glm::vec2(.0f, 1.f);
}

auto Arcadia::Glm::CreateUnitNegativeX() -> glm::vec2
{
    return -Vec2_CreateUnitPositiveX();
}

auto Arcadia::Glm::Vec2_CreateUnitNegativeY() -> glm::vec2
{
    return -Vec2_CreateUnitPositiveY();
}

auto Arcadia::Glm::Vec2_NormalizeFixedly(const glm::vec2& vec, glm::vec2::length_type index) -> glm::vec2
{
    ACDA_ASSERT(index >= 0 && index < 2);

    auto fixed = vec[index];
    auto a = vec[1 - index];

    if(a != 0)
    {
        a = boost::math::sign(a) * std::sqrt(1 - fixed * fixed);
    }

    glm::vec2 res{};
    res[index] = fixed;
    res[1 - index] = a;
    return res;
}

auto Arcadia::Glm::Int32Vec2_ToJson(const glm::i32vec2& vec) -> nlohmann::json
{
    return nlohmann::json::array(
        {
            vec.x,
            vec.y
        }
    );
}

auto Arcadia::Glm::Int32Vec2_FromJson(const nlohmann::json& json) -> glm::i32vec2
{
    return glm::i32vec2(
        json[0],
        json[1]
    );
}

auto Arcadia::Glm::Vec3_ToJson(const glm::vec3& vec) -> nlohmann::json
{
    return nlohmann::json::array(
        {
            vec.x,
            vec.y,
            vec.z
        }
    );
}

auto Arcadia::Glm::Vec3_FromJson(const nlohmann::json& json) -> glm::vec3
{
    return glm::vec3(
        json[0],
        json[1],
        json[2]
    );
}

auto Arcadia::Glm::Vec3_CreateZero() -> glm::vec3
{
    return glm::vec3{};
}


auto Arcadia::Glm::Vec3_CreateUnitPositiveX() -> glm::vec3
{
    return glm::vec3(1.f, .0f, .0f);
}

auto Arcadia::Glm::Vec3_CreateUnitPositiveY() -> glm::vec3
{
    return glm::vec3(.0f, 1.f, .0f);
}

auto Arcadia::Glm::Vec3_CreateUnitPositiveZ() -> glm::vec3
{
    return glm::vec3(.0f, .0f, 1.f);
}

auto Arcadia::Glm::Vec3_CreateUnitNegativeX() -> glm::vec3
{
    return -Vec3_CreateUnitPositiveX();
}

auto Arcadia::Glm::Vec3_CreateUnitNegativeY() -> glm::vec3
{
    return -Vec3_CreateUnitPositiveY();
}

auto Arcadia::Glm::Vec3_CreateUnitNegativeZ() -> glm::vec3
{
    return -Vec3_CreateUnitPositiveZ();
}

auto Arcadia::Glm::Vec3_NormalizeFixedly(const glm::vec3& vec, glm::vec3::length_type index) -> glm::vec3
{
    ACDA_ASSERT(index >= 0 && index < 3);

    auto fixed = vec[index];
    auto a = vec[(index + 1) % 3];
    auto b = vec[(index + 2) % 3];

    auto R = std::sqrt(1 - fixed * fixed); // Radius of target cicle
    auto r = std::sqrt(a * a + b * b); // Radius of the circle point (a,b) is on
    if(r != 0)
    {
        a = a * R / r;
        b = b * R / r;
    }
    else
    {
        a = R;
        b = 0;
    }

    glm::vec3 res{};
    res[index] = fixed;
    res[(index + 1) % 3] = a;
    res[(index + 2) % 3] = b;
    return res;
}

auto Arcadia::Glm::Vec3_NormalizeFixedly(const glm::vec3& compare, const glm::vec3& vec) -> glm::vec3
{
    if(compare.x != vec.x)
    {
        return Vec3_NormalizeFixedly(vec, 0);
    }
    else if(compare.y != vec.y)
    {
        return Vec3_NormalizeFixedly(vec, 1);
    }
    else if(compare.z != vec.z)
    {
        return Vec3_NormalizeFixedly(vec, 2);
    }
    return vec;
}

auto Arcadia::Glm::Int32Vec3_ToJson(const glm::i32vec3& vec) -> nlohmann::json
{
    return nlohmann::json::array(
        {
             vec.x,
             vec.y,
             vec.z
        }
    );
}

auto Arcadia::Glm::Int32Vec3_FromJson(const nlohmann::json& json) -> glm::i32vec3
{
    return glm::i32vec3(
        json[0],
        json[1],
        json[2]
    );
}

auto Arcadia::Glm::Vec4_ToJson(const glm::vec4& vec) -> nlohmann::json
{
    return nlohmann::json::array(
        {
            vec.x,
            vec.y,
            vec.z,
            vec.w
        }
    );
}

auto Arcadia::Glm::Vec4_FromJson(const nlohmann::json& json) -> glm::vec4
{
    return glm::vec4(
        json[0],
        json[1],
        json[2],
        json[3]
    );
}

auto Arcadia::Glm::Vec4_CreateZero() -> glm::vec4
{
    return glm::vec4{};
}

auto Arcadia::Glm::Vec4_NormalizeFixedly(const glm::vec4& vec, glm::vec4::length_type index) -> glm::vec4
{
    ACDA_ASSERT(index >= 0 && index < vec.length());

    auto fixed = vec[index];
    auto a = vec[(index + 1) % 4];
    auto b = vec[(index + 2) % 4];
    auto c = vec[(index + 3) % 4];

    auto R = std::sqrt(1 - fixed * fixed); // Radius of target cicle
    auto r = std::sqrt(a * a + b * b + c * c); // Radius of the circle point (a,b,c) is on
    if(r != 0)
    {
        a = a * R / r;
        b = b * R / r;
        c = c * R / r;
    }
    else
    {
        a = R;
        b = 0;
        c = 0;
    }

    glm::vec4 res{};
    res[index] = fixed;
    res[(index + 1) % 4] = a;
    res[(index + 2) % 4] = b;
    res[(index + 3) % 4] = c;
    return res;
}
