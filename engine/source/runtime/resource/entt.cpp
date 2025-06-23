

#include "entt.hpp"

auto Arcadia::ToString(entt::entity entt_entity) -> std::string
{
    return std::to_string(static_cast<entt::id_type>(entt_entity));
}
