#pragma once

#include <string>

#include "entt/entt.hpp"

#include "core/enum.hpp"
#include "platform/api_def.hpp"

namespace Arcadia
{
    static inline auto ToString(entt::entity entt_entity) -> std::string
    {
        return std::to_string(static_cast<entt::id_type>(entt_entity));
    }
}

namespace std
{
    template<>
    class formatter<entt::entity>: public std::formatter<std::string>
    {
    public:
        auto format(const entt::entity& entt_entity, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{}", Arcadia::ToUnderlying(entt_entity)),
                ctx
            );
        }
    };
}