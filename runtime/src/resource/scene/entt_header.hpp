#pragma once

#include<string>

#include"entt/entt.hpp"

#include"core/base.hpp"

namespace Arcadia
{
    static inline auto ToString(const entt::entity entity) -> std::string
    {
        return std::to_string(static_cast<entt::id_type>(entity));
    }
}

namespace std
{
    template<>
    struct std::formatter<entt::entity>: std::formatter<std::string>
    {
        auto format(const entt::entity& entity, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{}", Arcadia::to_underlying(entity)),
                ctx
            );
        }
    };
}
