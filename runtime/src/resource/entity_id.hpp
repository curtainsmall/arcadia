#pragma once

#include "resource/entt.hpp"

namespace Arcadia
{
    struct EntityId
    {
    public:
        EntityId() = default;
        EntityId(entt::entity id);

        auto operator=(entt::entity id);
        auto operator==(const EntityId& rhs) const -> bool;

        operator bool() const;
        operator entt::entity() const;
        auto GetValue() const->entt::entity;

        void SetNull();
        auto IsNull() const -> bool;

    private:
        entt::entity _Id{ entt::null };
    };
}

namespace std
{
    template<>
    struct hash<Arcadia::EntityId>
    {
    public:
        auto operator()(const Arcadia::EntityId& entity_id) const->std::size_t
        {
            return std::hash<entt::entity>()(entity_id.GetValue());
        }
    };

    template<>
    struct formatter<Arcadia::EntityId>: public std::formatter<std::string>
    {
    public:
        auto format(const Arcadia::EntityId& entity_id, std::format_context& ctx) const
        {
            return std::formatter<std::string>::format(
                std::format("{}", entity_id.GetValue()),
                ctx
            );
        }
    };
}