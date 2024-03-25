#pragma once

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace arcadia
{
    namespace vec4
    {
        [[nodiscard]]
        ARCADIA_API inline auto to_json(const glm::vec4& vec) -> nlohmann::json
        {
            return nlohmann::json{
                {"x",vec.x},
                {"y",vec.y},
                {"z",vec.z},
                {"w",vec.w}
            };
        }
        [[nodiscard]]
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::vec4
        {
            return glm::vec4{
                json.at("x"),
                json.at("y"),
                json.at("z"),
                json.at("w")
            };
        }

        [[nodiscard]]
        ARCADIA_API constexpr auto zero() -> glm::vec4
        {
            return glm::vec4{};
        }

        /// @brief Normalize vector with one axis fixed
        /// @param vec Vector to normalize
        /// @param fixed_index Index of fixed axis, must be 0, 1, 2 or 3
        /// @return Normalized vector
        [[nodiscard]]
        ARCADIA_API inline auto fixed_normalize(const glm::vec4& vec, glm::vec4::length_type fixed_index) -> glm::vec4
        {
            ARCADIA_ASSERT(fixed_index >= 0 && fixed_index < vec.length());

            auto fixed = vec[fixed_index];
            auto a = vec[(fixed_index + 1) % 4];
            auto b = vec[(fixed_index + 2) % 4];
            auto c = vec[(fixed_index + 3) % 4];

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
            res[fixed_index] = fixed;
            res[(fixed_index + 1) % 4] = a;
            res[(fixed_index + 2) % 4] = b;
            res[(fixed_index + 3) % 4] = c;
            return res;
        }
    }

    namespace dvec4
    {
        [[nodiscard]]
        ARCADIA_API inline auto to_json(const glm::dvec4& vec) -> nlohmann::json
        {
            return nlohmann::json{
                {"x",vec.x},
                {"y",vec.y},
                {"z",vec.z},
                {"w",vec.w}
            };
        }
        [[nodiscard]]
        ARCADIA_API inline auto from_json(const nlohmann::json& json) -> glm::dvec4
        {
            return glm::dvec4{
                json.at("x"),
                json.at("y"),
                json.at("z"),
                json.at("w")
            };
        }

        [[nodiscard]]
        ARCADIA_API constexpr auto zero() -> glm::dvec4
        {
            return glm::dvec4{};
        }

        /// @brief Normalize vector with one axis fixed
        /// @param vec Vector to normalize
        /// @param fixed_index Index of fixed axis, must be 0, 1, 2 or 3
        /// @return Normalized vector
        [[nodiscard]]
        ARCADIA_API inline auto fixed_normalize(const glm::dvec4& vec, glm::dvec4::length_type fixed_index) -> glm::dvec4
        {
            ARCADIA_ASSERT(fixed_index >= 0 && fixed_index < vec.length());

            auto fixed = vec[fixed_index];
            auto a = vec[(fixed_index + 1) % 4];
            auto b = vec[(fixed_index + 2) % 4];
            auto c = vec[(fixed_index + 3) % 4];

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

            glm::dvec4 res{};
            res[fixed_index] = fixed;
            res[(fixed_index + 1) % 4] = a;
            res[(fixed_index + 2) % 4] = b;
            res[(fixed_index + 3) % 4] = c;
            return res;
        }
    }
}

namespace std
{
    template<>
    struct std::formatter<glm::vec4>: std::formatter<std::string>
    {
        auto format(const glm::vec4& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2f}, {:.2f}, {:.2f}, {:.2f}", vec.x, vec.y, vec.z, vec.w),
                ctx
            );
        }
    };
}
