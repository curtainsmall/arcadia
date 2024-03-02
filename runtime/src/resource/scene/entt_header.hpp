#pragma once

#include<string>

#include"entt/entt.hpp"

namespace arcadia
{
    inline auto to_string(const entt::entity entity) -> std::string
    {
        return std::to_string(static_cast<entt::id_type>(entity));
    }
}
