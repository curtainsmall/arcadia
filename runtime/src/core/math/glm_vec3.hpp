#pragma once

#include "core/math/glm.hpp"
#include "core/nlohmann_json.hpp"
#include "platform/api_def.hpp"

namespace Arcadia
{
    namespace GlmVec3
    {
        [[nodiscard]]
        ACDA_API static inline auto ToJson(const glm::vec3& vec) -> nlohmann::json
        {
            return nlohmann::json::array(
                {
                    vec.x,
                    vec.y,
                    vec.z
                }
            );
        }
        [[nodiscard]]
        ACDA_API static inline auto FromJson(const nlohmann::json& json) -> glm::vec3
        {
            return glm::vec3(
                json[0],
                json[1],
                json[2]
            );
        }

        [[nodiscard]]
        ACDA_API constexpr auto CreateZero() -> glm::vec3
        {
            return glm::vec3{};
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateUnitPositiveX() -> glm::vec3
        {
            return glm::vec3(1.f, .0f, .0f);
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateUnitPositiveY() -> glm::vec3
        {
            return glm::vec3(.0f, 1.f, .0f);
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateUnitPositiveZ() -> glm::vec3
        {
            return glm::vec3(.0f, .0f, 1.f);
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateUnitNegativeX() -> glm::vec3
        {
            return -CreateUnitPositiveX();
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateUnitNegativeY() -> glm::vec3
        {
            return -CreateUnitPositiveY();
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateUnitNegativeZ() -> glm::vec3
        {
            return -CreateUnitPositiveZ();
        }

        template<std::size_t Index>
            requires (Index >= 0) && (Index <= 2)
        [[nodiscard]]
        ACDA_API static inline auto NormalizeFixedly(const glm::vec3& vec) -> glm::vec3
        {
            static_assert(Index >= 0 && Index < 3);

            auto fixed = vec[Index];
            auto a = vec[(Index + 1) % 3];
            auto b = vec[(Index + 2) % 3];

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
            res[Index] = fixed;
            res[(Index + 1) % 3] = a;
            res[(Index + 2) % 3] = b;
            return res;
        }

        [[nodiscard]]
        ACDA_API static inline auto NormalizeFixedly(const glm::vec3& compare, const glm::vec3& vec) -> glm::vec3
        {
            if(compare.x != vec.x)
            {
                return NormalizeFixedly<0>(vec);
            }
            else if(compare.y != vec.y)
            {
                return NormalizeFixedly<1>(vec);
            }
            else if(compare.z != vec.z)
            {
                return NormalizeFixedly<2>(vec);
            }
            return vec;
        }
    }

    namespace GlmInt32Vec3
    {
        [[nodiscard]]
        ACDA_API static inline auto ToJson(const glm::i32vec3& vec) -> nlohmann::json
        {
            return nlohmann::json::array(
                {
                     vec.x,
                     vec.y,
                     vec.z
                }
            );
        }
        [[nodiscard]]
        ACDA_API static inline auto FromJson(const nlohmann::json& json) -> glm::i32vec3
        {
            return glm::i32vec3(
                json[0],
                json[1],
                json[2]
            );
        }
    }
}

namespace std
{
    template<>
    class formatter<glm::vec3>: public std::formatter<std::string>
    {
    public:
        auto format(const glm::vec3& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2f}, {:.2f}, {:.2f}", vec.x, vec.y, vec.z),
                ctx
            );
        }
    };

    template<>
    class formatter<glm::i32vec3>: public std::formatter<std::string>
    {
    public:
        auto format(const glm::i32vec3& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2f}, {:.2f}, {:.2f}", vec.x, vec.y, vec.z),
                ctx
            );
        }
    };
}