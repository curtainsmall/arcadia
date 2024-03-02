#pragma once

#include<filesystem>
#include<string>
#include<unordered_map>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/exception.hpp"
#include"core/nlohmann_json_header.hpp"
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
        scene(const nlohmann::json& json);
        ~scene() = default;
        auto to_json() const->nlohmann::json;

        scene(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;

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

        [[nodiscard]]
        inline auto get_registry() const -> const entt::registry&
        {
            return _registry;
        }

    private:
        /// @throw invalid_entity if @a entity is invalid
        void _check_valid_entity_or_throw(const entt::entity entity) const;
    public:
        std::string name;
    private:
        entt::registry _registry{};
    };

    struct ARCADIA_API json_scene_add_component_to_entity_helper: arcadia::noncopyable
    {
    public:
        using self_type = json_scene_add_component_to_entity_helper;
    public:
        inline json_scene_add_component_to_entity_helper(const arcadia::scene& scene, nlohmann::json& json_entity):
            _scene_ptr(&scene),
            _json_entity_ptr(&json_entity)
        {}
        ~json_scene_add_component_to_entity_helper() = default;

        template<arcadia::component_like Component>
        auto add(const std::string& name) -> self_type&
        {
            auto view = _scene_ptr->get_registry().view<Component>();
            for(auto entity : view)
            {
                const auto& [comp] = view.get(entity);
                _json_entity_ptr
                    ->at(arcadia::to_string(entity))
                    .push_back(
                        { name , comp.to_json() }
                );
            }
            return *this;
        }
    private:
        const arcadia::scene* _scene_ptr;
        nlohmann::json* _json_entity_ptr;
    };
}
