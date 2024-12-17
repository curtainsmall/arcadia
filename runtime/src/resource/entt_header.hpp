#pragma once

#include<string>

#include"entt/entt.hpp"

#include"core/enum.hpp"
#include"platform/api_def.hpp"

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
    class formatter<entt::entity>: public std::formatter<std::string>
    {
    public:
        auto format(const entt::entity& entity, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{}", Arcadia::ToUnderlying(entity)),
                ctx
            );
        }
    };
}