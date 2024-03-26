#pragma once

#include"core/base.hpp"
#include"resource/scene/entt_header.hpp"

namespace arcadia
{
    struct ARCADIA_API entity: arcadia::noncopyable
    {
    public:
        using self_type = entity;
    public:
        entity() = default;
        inline entity(const entt::entity entity):
            _entity(entity)
        {}

        auto operator=(const entt::entity entity) -> self_type&
        {
            _entity = entity;
            return *this;
        }

        entity(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;

        [[nodiscard]]
        auto get() const -> entt::entity
        {
            return _entity;
        }

        operator entt::entity() const
        {
            return get();
        }
    private:
        entt::entity _entity{ entt::null };
    };
}
