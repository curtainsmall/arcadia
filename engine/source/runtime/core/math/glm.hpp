#pragma once

#define GLM_FORCE_CTOR_INIT
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_QUAT_DATA_WXYZ
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "platform/defines.hpp"
#include "core/nlohmann_json.hpp"

namespace Arcadia
{
    namespace Glm
    {
        ACDA_API void Decompose(
            const glm::mat4& transform,
            glm::vec3& translation,
            glm::vec3& rotation,
            glm::vec3& scale
        );

        [[nodiscard]]
        ACDA_API auto Mat2_ToJson(const glm::mat2& mat) -> nlohmann::json;
        [[nodiscard]]
        ACDA_API auto Mat2_FromJson(const nlohmann::json& json) -> glm::mat2;
        [[nodiscard]]
        ACDA_API auto Mat2_CreateZero() -> glm::mat2;
        [[nodiscard]]
        ACDA_API auto Mat2_CreateIdentity() -> glm::mat2;

        [[nodiscard]]
        ACDA_API auto Mat3_ToJson(const glm::mat3& mat) -> nlohmann::json;
        [[nodiscard]]
        ACDA_API auto Mat3_FromJson(const nlohmann::json& json) -> glm::mat3;
        [[nodiscard]]
        ACDA_API auto Mat3_CreateZero() -> glm::mat3;
        [[nodiscard]]
        ACDA_API auto Mat3_CreateIdentity() -> glm::mat3;

        [[nodiscard]]
        ACDA_API auto Mat4_ToJson(const glm::mat4& mat) -> nlohmann::json;
        [[nodiscard]]
        ACDA_API auto Mat4_FromJson(const nlohmann::json& json) -> glm::mat4;
        [[nodiscard]]
        ACDA_API auto Mat4_CreateZero() -> glm::mat4;
        [[nodiscard]]
        ACDA_API auto Mat4_CreateZeroAffine() -> glm::mat4;
        [[nodiscard]]
        ACDA_API auto Mat4_CreateIdentity() -> glm::mat4;

        [[nodiscard]]
        ACDA_API auto Quat_ToJson(const glm::quat& quat) -> nlohmann::json;
        [[nodiscard]]
        ACDA_API auto Quat_FromJson(const nlohmann::json& json) -> glm::quat;
        [[nodiscard]]
        ACDA_API auto Quat_CreateZero() -> glm::quat;
        [[nodiscard]]
        ACDA_API auto Quat_CreateIdentity() -> glm::quat;
        [[nodiscard]]
        ACDA_API auto Quat_NormalizeFixedly(const glm::quat& quat, glm::quat::length_type index) -> glm::quat;
        [[nodiscard]]
        ACDA_API auto Quat_NormalizeFixedly(const glm::quat& compare, const glm::quat& quat) -> glm::quat;

        [[nodiscard]]
        ACDA_API auto Vec2_ToJson(const glm::vec2& vec) -> nlohmann::json;
        [[nodiscard]]
        ACDA_API auto Vec2_FromJson(const nlohmann::json& json) -> glm::vec2;
        [[nodiscard]]
        ACDA_API auto Vec2_CreateZero() -> glm::vec2;
        [[nodiscard]]
        ACDA_API auto Vec2_CreateUnitPositiveX() -> glm::vec2;
        [[nodiscard]]
        ACDA_API auto Vec2_CreateUnitPositiveY() -> glm::vec2;
        [[nodiscard]]
        ACDA_API auto CreateUnitNegativeX() -> glm::vec2;
        [[nodiscard]]
        ACDA_API auto Vec2_CreateUnitNegativeY() -> glm::vec2;
        [[nodiscard]]
        ACDA_API auto Vec2_NormalizeFixedly(const glm::vec2& vec, glm::vec2::length_type index) -> glm::vec2;
        [[nodiscard]]
        ACDA_API auto Int32Vec2_ToJson(const glm::i32vec2& vec) -> nlohmann::json;
        [[nodiscard]]
        ACDA_API auto Int32Vec2_FromJson(const nlohmann::json& json) -> glm::i32vec2;

        [[nodiscard]]
        ACDA_API auto Vec3_ToJson(const glm::vec3& vec) -> nlohmann::json;
        [[nodiscard]]
        ACDA_API auto Vec3_FromJson(const nlohmann::json& json) -> glm::vec3;
        [[nodiscard]]
        ACDA_API auto Vec3_CreateZero() -> glm::vec3;
        [[nodiscard]]
        ACDA_API auto Vec3_CreateUnitPositiveX() -> glm::vec3;
        [[nodiscard]]
        ACDA_API auto Vec3_CreateUnitPositiveY() -> glm::vec3;
        [[nodiscard]]
        ACDA_API auto Vec3_CreateUnitPositiveZ() -> glm::vec3;
        [[nodiscard]]
        ACDA_API auto Vec3_CreateUnitNegativeX() -> glm::vec3;
        [[nodiscard]]
        ACDA_API auto Vec3_CreateUnitNegativeY() -> glm::vec3;
        [[nodiscard]]
        ACDA_API auto Vec3_CreateUnitNegativeZ() -> glm::vec3;
        [[nodiscard]]
        ACDA_API auto Vec3_NormalizeFixedly(const glm::vec3& vec, glm::vec3::length_type index) -> glm::vec3;
        [[nodiscard]]
        ACDA_API auto Vec3_NormalizeFixedly(const glm::vec3& compare, const glm::vec3& vec) -> glm::vec3;
        [[nodiscard]]
        ACDA_API auto Int32Vec3_ToJson(const glm::i32vec3& vec) -> nlohmann::json;
        [[nodiscard]]
        ACDA_API auto Int32Vec3_FromJson(const nlohmann::json& json) -> glm::i32vec3;

        [[nodiscard]]
        ACDA_API auto Vec4_ToJson(const glm::vec4& vec) -> nlohmann::json;
        [[nodiscard]]
        ACDA_API auto Vec4_FromJson(const nlohmann::json& json) -> glm::vec4;
        [[nodiscard]]
        ACDA_API auto Vec4_CreateZero() -> glm::vec4;

        [[nodiscard]]
        ACDA_API auto Vec4_NormalizeFixedly(const glm::vec4& vec, glm::vec4::length_type index) -> glm::vec4;
    }
}

namespace std
{
    template<>
    struct ACDA_API formatter<glm::quat>: public std::formatter<std::string>
    {
    public:
        auto format(const glm::quat& quat, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::vformat("{:.2f},{:.2f},{:.2f},{:.2f}", std::make_format_args(quat.w, quat.x, quat.y, quat.z)),
                ctx
            );
        }
    };

    template<>
    struct ACDA_API formatter<glm::vec2>:public std::formatter<std::string>
    {
    public:
        auto format(const glm::vec2& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::vformat("{:.2f}, {:.2f}", std::make_format_args(vec.x, vec.y)),
                ctx
            );
        }
    };

    template<>
    struct ACDA_API formatter<glm::i32vec2>: public std::formatter<std::string>
    {
    public:
        auto format(const glm::i32vec2& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::vformat("{:.2f}, {:.2f}", std::make_format_args(vec.x, vec.y)),
                ctx
            );
        }
    };

    template<>
    struct ACDA_API formatter<glm::vec3>: public std::formatter<std::string>
    {
    public:
        auto format(const glm::vec3& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::vformat("{:.2f}, {:.2f}, {:.2f}", std::make_format_args(vec.x, vec.y, vec.z)),
                ctx
            );
        }
    };

    template<>
    struct ACDA_API formatter<glm::i32vec3>: public std::formatter<std::string>
    {
    public:
        auto format(const glm::i32vec3& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::vformat("{:.2f}, {:.2f}, {:.2f}", std::make_format_args(vec.x, vec.y, vec.z)),
                ctx
            );
        }
    };

    template<>
    struct ACDA_API formatter<glm::vec4>: public std::formatter<std::string>
    {
    public:
        auto format(const glm::vec4& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::vformat("{:.2f}, {:.2f}, {:.2f}, {:.2f}", std::make_format_args(vec.x, vec.y, vec.z, vec.w)),
                ctx
            );
        }
    };
}