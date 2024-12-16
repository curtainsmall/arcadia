#pragma once

#include"boost/math/special_functions.hpp"

#include"core/base.hpp"
#include"core/math/glm_header.hpp"
#include"core/nlohmann_json_header.hpp"

namespace Arcadia
{
    namespace GlmVec2
    {
        [[nodiscard]]
        ACDA_API static inline auto ToJson(const glm::vec2& vec) -> nlohmann::json
        {
            return nlohmann::json{
                 { "x",vec.x },
                 { "y",vec.y }
            };
        }
        [[nodiscard]]
        ACDA_API static inline auto FromJson(const nlohmann::json& json) -> glm::vec2
        {
            return glm::vec2(
                json.at("x"),
                json.at("y")
            );
        }

        [[nodiscard]]
        ACDA_API constexpr auto CreateZero() -> glm::vec2
        {
            return glm::vec2{};
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateUnitPositiveX() -> glm::vec2
        {
            return glm::vec2(1.f, .0f);
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateUnitPositiveY() -> glm::vec2
        {
            return glm::vec2(.0f, 1.f);
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateUnitNegativeX() -> glm::vec2
        {
            return -CreateUnitPositiveX();
        }
        [[nodiscard]]
        ACDA_API constexpr auto CreateUnitNegativeY() -> glm::vec2
        {
            return -CreateUnitPositiveY();
        }

        /// @brief Normalize vector with one axis fixed
        /// @tparam Index Index of fixed axis, must be 0 or 1
        /// @param vec Vector to normalize
        /// @return Normalized vector
        template<std::size_t Index>
        [[nodiscard]]
        ACDA_API static inline auto NormalizeFixedly(const glm::vec2& vec) -> glm::vec2
        {
            static_assert(Index >= 0 && Index < 2);

            auto fixed = vec[Index];
            auto a = vec[1 - Index];

            if(a != 0)
            {
                a = boost::math::sign(a) * std::sqrt(1 - fixed * fixed);
            }

            glm::vec2 res{};
            res[Index] = fixed;
            res[1 - Index] = a;
            return res;
        }
    }

    namespace GlmInt32Vec2
    {
        [[nodiscard]]
        ACDA_API static inline auto ToJson(const glm::i32vec2& vec) -> nlohmann::json
        {
            return nlohmann::json{
                {"x",vec.x},
                {"y",vec.y}
            };
        }
        [[nodiscard]]
        ACDA_API static inline auto FromJson(const nlohmann::json& json) -> glm::i32vec2
        {
            return glm::i32vec2(
                    json.at("x"),
                    json.at("y")
            );
        }
    }
}

namespace std
{
    template<>
    class formatter<glm::vec2>:public std::formatter<std::string>
    {
    public:
        auto format(const glm::vec2& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2f}, {:.2f}", vec.x, vec.y),
                ctx
            );
        }
    };

    template<>
    class formatter<glm::i32vec2>: public std::formatter<std::string>
    {
    public:
        auto format(const glm::i32vec2& vec, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{:.2f}, {:.2f}", vec.x, vec.y),
                ctx
            );
        }
    };
}