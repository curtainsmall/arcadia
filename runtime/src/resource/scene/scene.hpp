#pragma once

#include<filesystem>
#include<string>
#include<unordered_map>

#include"boost/bimap.hpp"

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/exception.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/uuid.hpp"
#include"resource/component/component.hpp"
#include"resource/scene/entt_header.hpp"

namespace arcadia
{
    struct ARCADIA_API entity_info: arcadia::noncopyable
    {
    public:
        using self_type = entity_info;
    public:
        entity_info() = default;
        entity_info(const nlohmann::json& json);
        [[nodiscard]]
        auto to_json() const->nlohmann::json;
    public:
        bool should_render_in_viewport{ true };
    };

    struct scene;
    struct ARCADIA_API scene_memento_data
    {
        friend struct arcadia::scene;
    private:
        struct ARCADIA_API emplaced_entity_info
        {
        public:
            using self_type = emplaced_entity_info;
        public:
            std::string name{};
        };
        struct ARCADIA_API replaced_entity_info
        {
        public:
            using self_type = replaced_entity_info;
        public:
            std::string prev_name{};
            std::string current_name{};

        };

    private:
        std::string name;
    };

    struct ARCADIA_API scene: arcadia::noncopyable
    {
    public:
        using registry_type = entt::registry;
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
        auto get_name() const -> const std::string&;
        void set_name(const std::string& name);

        [[nodiscard]]
        inline auto get_registry() const -> const registry_type&
        {
            return _registry;
        }

        [[nodiscard]]
        inline auto get_name_entity_bimap() const -> const boost::bimap<std::string, entt::entity>
        {
            return _name_entity_bimap;
        }
        [[nodiscard]]
        auto get_name_of_entity(const entt::entity entity) const -> const std::string&;
        [[nodiscard]]
        auto get_entity_of_name(const std::string& name) const->entt::entity;

        [[nodiscard]]
        auto get_entity_info(const entt::entity entity) const -> const arcadia::entity_info&;
        [[nodiscard]]
        auto get_entity_info(const entt::entity entity) -> arcadia::entity_info&;

        /// @brief Rename entity
        /// @param old_name Old name
        /// @param new_name New name
        /// @return Whether succeeded
        auto rename(const std::string& old_name, const std::string& new_name) -> bool;

        /// @brief Check whether there is an entity with given name
        /// @param name Name of entity
        /// @return Result
        [[nodiscard]]
        auto contains(const std::string& name) const -> bool;

        /// @brief Create an new entity
        /// @param name Name of the entity
        /// @return Created entity
        [[nodiscard]]
        auto create(const std::string& name) -> entt::entity;

        /// @brief Create an new entity
        /// @param name Name of the entity
        /// @param json_entity_info Json used to contruct @ref arcadia::entity_info
        /// @return Created entity
        [[nodiscard]]
        auto create(const std::string& name, const nlohmann::json& json_entity_info) -> entt::entity;

        /// @brief Destroy entity
        /// @param entity Entity to destroy
        /// @return The version of recycled entity
        auto destroy(const entt::entity entity) -> registry_type::version_type;

        /// @brief Check whether entity is valid
        /// @param entity Entity to check
        /// @return Result
        [[nodiscard]]
        auto is_valid(const entt::entity entity) const -> bool;

        /// @brief Emplace a component to an entity
        /// @tparam ...Args Types of arguments
        /// @tparam Component Type of component
        /// @param entity Entity to emplace component to
        /// @param ...args Arguments for constructing component
        /// @return Emplaced component
        template<arcadia::component_like Component, class ...Args>
        auto emplace(const entt::entity entity, Args&& ...args) -> Component&
        {
            auto& comp = _registry.emplace<Component>(entity, std::forward<Args>(args)...);
            return comp;
        }

        /// @brief Replace component in the entity
        /// @tparam ...Args Types of arguments
        /// @tparam Component Type of component
        /// @param entity Entity to replace component to
        /// @param ...args Arguments for constructing component
        /// @return Replaced component
        template<arcadia::component_like Component, class ...Args>
        auto replace(const entt::entity entity, Args&& ...args) -> Component&
        {
            ARCADIA_ASSERT(all_of<Component>(entity));

            auto& comp = _registry.replace<Component>(entity, std::forward<Args>(args)...);
            return comp;
        }

        /// @brief Emplace or replace component in the entity
        /// @tparam ...Args Types of arguments
        /// @tparam Component Type of component
        /// @param entity Entity to replace component to
        /// @param ...args Arguments for constructing component
        /// @return Emplaced/replaced component
        template<arcadia::component_like Component, class ...Args>
        auto emplace_or_replace(const entt::entity entity, Args&& ...args) -> Component&
        {
            auto& comp = _registry.emplace_or_replace<Component>(entity, std::forward<Args>(args)...);
            return comp;
        }

        /// @brief Get component in the entity
        /// @tparam ...Components Types of component
        /// @param entity Entity to get component from
        /// @return Got component(s)
        template<arcadia::component_like ...Components>
        [[nodiscard]]
        auto get(const entt::entity entity) const -> decltype(auto)
        {
            ARCADIA_ASSERT(all_of<Components...>(entity));

            return _registry.get<Components...>(entity);
        }

        /// @copydoc arcadia::scece::get
        template<arcadia::component_like ...Components>
        [[nodiscard]]
        auto get(const entt::entity entity) -> decltype(auto)
        {
            ARCADIA_ASSERT(all_of<Components...>(entity));

            auto& comp = _registry.get<Components...>(entity);
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

        /// @brief Remove component from entity
        /// @tparam ...Component Type of component
        /// @param entity Entity to remove component from
        template<arcadia::component_like ...Component>
        auto remove(const entt::entity entity) -> registry_type::size_type
        {
            auto count = _registry.remove<Component...>(entity);
            return count;
        }

        /// @brief Get a view for the components
        /// @tparam ...Components Types of component used to construct the view
        /// @tparam ...ExcludeComponents Types of component used to filter the view
        /// @param exclude Helper class to specify @ref ...ExcludeComponents
        /// @return Created view
        template<arcadia::component_like ...Components, arcadia::component_like ...ExcludeComponents>
        [[nodiscard]]
        auto view(entt::exclude_t<ExcludeComponents...> exclude = entt::exclude_t{}) -> decltype(auto)
        {
            auto view = _registry.view<Components...>(exclude);
            return view;
        }

        /// @copydoc arcadia::scene::view
        template<arcadia::component_like ...Components, arcadia::component_like ...ExcludeComponents>
        [[nodiscard]]
        auto view(entt::exclude_t<ExcludeComponents...> exclude= entt::exclude_t{}) const -> decltype(auto)
        {
            return _registry.view<Components...>(exclude);
        }

        /// @brief Get a group for the components
        /// @tparam ...OwnedComponents Types of component owned by the group
        /// @tparam ...GetComponents Types of component observed by the group, if any
        /// @tparam ...ExcludeComponents Types of component used to filter the group, if any
        /// @param get Helper class to specify @ref ...GetComponents
        /// @param exclude Helper class to specify @ref ...ExcludeComponents
        /// @return 
        template<arcadia::component_like ...OwnedComponents, arcadia::component_like ...GetComponents, arcadia::component_like ...ExcludeComponents>
        [[nodiscard]]
        auto group(entt::get_t<GetComponents...> get = entt::get_t{}, entt::exclude_t<ExcludeComponents...> exclude= entt::exclude_t{}) -> decltype(auto)
        {
            auto group = _registry.group<OwnedComponents...>(get, exclude);
            return group;
        }

    private:
        auto _create_json_components(const entt::entity entity) const->nlohmann::json;
    private:
        std::string _name;

        boost::bimap<std::string, entt::entity> _name_entity_bimap{};
        std::unordered_map<entt::entity, arcadia::entity_info> _entity_info_umap{};
        entt::registry _registry{};
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

        template<arcadia::component_like Component>
        auto add(const std::string& type) -> self_type&
        {
            for(auto [entity, comp] : _scene_ptr->view<Component>().each())
            {
                _json_entities_ptr
                    ->at(_scene_ptr->get_name_of_entity(entity))
                    .push_back({ type , comp.to_json() });
            }
            return *this;
        }
    private:
        const arcadia::scene* _scene_ptr;
        nlohmann::json* _json_entities_ptr;
    };
}
