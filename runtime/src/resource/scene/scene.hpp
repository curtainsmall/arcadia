#pragma once

#include<filesystem>
#include<string>
#include<unordered_map>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/exception.hpp"
#include"core/uuid.hpp"
#include"resource/component/component.hpp"
#include"resource/scene/entt_header.hpp"

namespace arcadia
{
    struct ARCADIA_API scene: arcadia::noncopyable
    {
    public:
        ARCADIA_EXCEPTION(invalid_entity);
        ARCADIA_EXCEPTION(no_such_component);

        using self_type = scene;
    public:
        inline scene(const std::string& name):
            name(name)
        {}
        ~scene() = default;

        auto create_entity(const std::string& name ={}) -> entt::entity;
        auto destroy_entity(const entt::entity entity) -> entt::registry::version_type;
        auto is_valid(const entt::entity entity) const -> bool;

        template<arcadia::component_like Component, class ...Args>
        auto emplace_component(const entt::entity entity, Args&& ...args) -> Component&
        {
            return _registry.emplace<Component>(entity, std::forward<Args>(args)...);
        }

        template<arcadia::component_like Component, class ...Args>
        auto replace_component(const entt::entity entity, Args&& ...args) -> Component&
        {
            return _registry.replace<Component>(entity, std::forward<Args>(args)...);
        }

        template<arcadia::component_like Component, class ...Args>
        auto emplace_or_replace_component(const entt::entity entity, Args&& ...args) -> Component&
        {
            return _registry.emplace_or_replace(entity, std::forward<Args>(args)...);
        }

        template<arcadia::component_like Component>
        auto get_component(const entt::entity entity) -> Component&
        {
            if(!has_component<Component>(entity))
            {
                throw no_such_component{ std::format("No such component with entity: {0} with type info: {1}", static_cast<entt::id_type>(entity),typeid(Component)) };
            }
            return _registry.get<Component>(entity);
        }

        template<arcadia::component_like ...Components>
        auto has_component(const entt::entity entity) const -> bool
        {
            return _registry.all_of<Components...>(entity);
        }

        template<arcadia::component_like Component>
        void remove_component(const entt::entity entity)
        {
            _registry.remove<Component>(entity);
        }

    private:
        /// @throw invalid_entity if @a entity is invalid
        void _check_valid_entity_or_throw(const entt::entity entity) const;

    public:
        std::string name;
    private:
        entt::registry _registry{};
    };
}
