#pragma once

#include<any>
#include<filesystem>
#include<map>
#include<string>
#include<unordered_set>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/exception.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/uuid.hpp"
#include"resource/component/component.hpp"
#include"resource/component/meta_components/meta_componts.hpp"
#include"resource/scene/entt_header.hpp"

namespace arcadia
{
    struct ARCADIA_API scene: arcadia::noncopyable
    {
    public:
        ARCADIA_EXCEPTION(invalid_entity);
        ARCADIA_EXCEPTION(no_such_component);
        ARCADIA_EXCEPTION(conflict_name);

        using self_type = scene;
    public:

        inline scene(const std::string& name):
            _name(name)
        {}
        scene(const nlohmann::json& json);
        ~scene() = default;
        auto to_json() const->nlohmann::json;

        scene(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;

        [[nodiscard]]
        inline auto get_name() const -> const std::string&
        {
            return _name;
        }
        inline auto set_name(const std::string& name) -> self_type&
        {
            _name = name;
            _modified = true;
            return *this;
        }

        [[nodiscard]]
        inline auto is_modified() const -> bool
        {
            return _modified;
        }
        inline void set_modified(bool modified)
        {
            _modified = modified;
        }

        [[nodiscard]]
        inline auto get_registry() const -> const entt::registry&
        {
            return _registry;
        }

        [[nodiscard]]
        auto create(const std::string& name) -> entt::entity;
        auto destroy(const entt::entity entity) -> entt::registry::version_type;
        [[nodiscard]]
        auto valid(const entt::entity entity) const -> bool;

        template<arcadia::component_like Component, class ...Args>
        auto emplace(const entt::entity entity, Args&& ...args) -> Component&
        {
            auto& comp = _registry.emplace<Component>(entity, std::forward<Args>(args)...);
            set_modified(true);
            return comp;
        }

        template<arcadia::component_like Component, class ...Args>
        auto replace(const entt::entity entity, Args&& ...args) -> Component&
        {
            auto& comp = _registry.replace<Component>(entity, std::forward<Args>(args)...);
            set_modified(true);
            return comp;
        }

        template<arcadia::component_like Component, class ...Args>
        auto emplace_or_replace(const entt::entity entity, Args&& ...args) -> Component&
        {
            auto& comp = _registry.emplace_or_replace<Component>(entity, std::forward<Args>(args)...);
            set_modified(true);
            return comp;
        }

        template<arcadia::component_like ...Components>
        [[nodiscard]]
        auto get(const entt::entity entity) const -> decltype(auto)
        {
            if(!all_of<Components...>(entity))
            {
                throw no_such_component{ std::format("No such component with entity: {0}", static_cast<entt::id_type>(entity)) };
            }
            return _registry.get<Components...>(entity);
        }

        template<arcadia::component_like ...Components>
        [[nodiscard]]
        auto get(const entt::entity entity) -> decltype(auto)
        {
            if(!all_of<Components...>(entity))
            {
                throw no_such_component{ std::format("No such component with entity: {0}", static_cast<entt::id_type>(entity)) };
            }
            auto& comp = _registry.get<Components...>(entity);
            set_modified(true);
            return comp;
        }

        template<arcadia::component_like ...Components>
        [[nodiscard]]
        auto all_of(const entt::entity entity) const -> bool
        {
            return _registry.all_of<Components...>(entity);
        }

        template<arcadia::component_like ...Components>
        [[nodiscard]]
        auto any_of(const entt::entity entity) const -> bool
        {
            return _registry.any_of<Components...>(entity);
        }

        template<arcadia::component_like Component>
        void remove(const entt::entity entity)
        {
            _registry.remove<Component>(entity);
            set_modified(true);
        }

        template<arcadia::component_like ...Components, arcadia::component_like ...ExcludeComponents>
        [[nodiscard]]
        auto view(entt::exclude_t<ExcludeComponents...> exclude = entt::exclude_t{}) -> decltype(auto)
        {
            return _registry.view<Components...>(exclude);
        }

        template<arcadia::component_like ...Components, arcadia::component_like ...ExcludeComponents>
        [[nodiscard]]
        auto view(entt::exclude_t<ExcludeComponents...> exclude= entt::exclude_t{}) const -> decltype(auto)
        {
            return _registry.view<Components...>(exclude);
        }

        template<arcadia::component_like ...OwnedComponents, arcadia::component_like ...GetComponents, arcadia::component_like ...ExcludeComponents>
        [[nodiscard]]
        auto group(entt::get_t<GetComponents...> get = entt::get_t{}, entt::exclude_t<ExcludeComponents...>exclude= entt::exclude_t{}) -> decltype(auto)
        {
            return _registry.group<OwnedComponents...>(get, exclude);
        }

    private:
        /// @throw invalid_entity if @a entity is invalid
        void _check_valid_entity_or_throw(const entt::entity entity) const;
    private:
        bool _modified{ false };
        std::string _name;
        entt::registry _registry{};
        std::unordered_set<std::string> _name_uset{};
    };

    struct ARCADIA_API json_scene_add_component_to_entity_helper: arcadia::noncopyable
    {
    public:
        using self_type = json_scene_add_component_to_entity_helper;
    public:
        inline json_scene_add_component_to_entity_helper(const arcadia::scene& scene, nlohmann::json& json_entities):
            _scene_ptr(&scene),
            _json_entities_ptr(&json_entities)
        {}
        ~json_scene_add_component_to_entity_helper() = default;

        template<arcadia::component_like Components>
        auto add(const std::string& type) -> self_type&
        {
            auto view = _scene_ptr->get_registry().view<Components>();
            for(auto entity : view)
            {
                const auto& [comp] = view.get(entity);

                _json_entities_ptr
                    ->at(_scene_ptr->get<arcadia::name_component>(entity).get())
                    .push_back({ type , comp.to_json() });
            }
            return *this;
        }
    private:
        const arcadia::scene* _scene_ptr;
        nlohmann::json* _json_entities_ptr;
    };
}
