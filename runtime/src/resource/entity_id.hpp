#pragma once

#include"resource/entt_header.hpp"

namespace Arcadia
{
    class EntityId
    {
    public:
        EntityId() = default;
        EntityId(entt::entity id) :
            _Id(id)
        {}

        inline auto operator=(entt::entity id)
        {
            _Id = id;
        }

        inline auto operator==(const EntityId& rhs) const -> bool
        {
            return _Id == rhs._Id;
        }

        inline operator bool() const
        {
            return !IsNull();
        }

        inline operator entt::entity() const
        {
            return GetValue();
        }

        inline auto GetValue() const -> entt::entity
        {
            return _Id;
        }

        inline void SetNull()
        {
            _Id = entt::null;
        }

        inline auto IsNull() const -> bool
        {
            return _Id == entt::null;
        }

    private:
        entt::entity _Id{ entt::null };
    };
}

namespace std
{
    template<>
    class hash<Arcadia::EntityId>
    {
    public:
        auto operator()(const Arcadia::EntityId& entity_id) const->std::size_t
        {
            return std::hash<entt::entity>()(entity_id.GetValue());
        }
    };

    template<>
    class formatter<Arcadia::EntityId>: public std::formatter<std::string>
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