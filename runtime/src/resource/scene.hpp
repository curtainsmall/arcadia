#pragma once

#include<filesystem>
#include<string>
#include<unordered_map>

#include"core/assert.hpp"
#include"core/event/event.hpp"
#include"core/exception.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/noncopyable.hpp"
#include"core/string.hpp"
#include"core/uuid.hpp"
#include"platform/api_def.hpp"
#include"resource/components/component_interface.hpp"
#include"resource/entt_header.hpp"

namespace Arcadia
{
    static inline std::array BuildinEntityTypes{
         std::string("actor"),
         std::string("camera"),
         std::string("light"),
    };

    class EntityInfo
    {
        friend class Scene;
    public:
        using SelfType = EntityInfo;
    public:
        EntityInfo(
            const std::string& name,
            const entt::entity entity
        ) :
            _Name(name),
            _Entity(entity)
        {}

        [[nodiscard]]
        auto GetName() const -> const std::string&;

        [[nodiscard]]
        auto GetEntity() const->entt::entity;

    public:
        std::string Type{}; // Type of the entity
        bool Display{ true }; // Whether the entity will be displayed in the viewport (the renderer will skip the hidden ones)
        bool Internal{ false }; // Whether the entity is controled internally (it will not be listed in the outliner); Note that an internal entity will still be rendered unless `Display` is set to false
    private:
        std::string _Name{};
        entt::entity _Entity{};
    };

    class Scene: public Noncopyable
    {
    public:
        using EntityInfoStorageType = std::unordered_map<std::string, EntityInfo>;
        using RegistryType = entt::registry;
        using SelfType = Scene;
    public:
        Scene(const std::string& name) :
            Name(name)
        {}
        Scene(const nlohmann::json& json);
        ~Scene() = default;
        auto ToJson() const->nlohmann::json;

        Scene(SelfType&&) noexcept = default;
        auto operator=(SelfType&&) noexcept -> SelfType & = default;

        [[nodiscard]]
        auto GetRegistry() const -> const RegistryType&
        {
            return _Registry;
        }

        void RenameEntity(const std::string& name, const std::string& new_name);

        /// @brief Check whether there is an entity with given name
        /// @param name Name of entity
        /// @return Result
        [[nodiscard]]
        auto ContainsEntity(const std::string& name) const -> bool;

        [[nodiscard]]
        auto GetSize() const->std::size_t;

        [[nodiscard]]
        auto CountEntity(const std::function<bool(const std::string&, const EntityInfo&)>& pred) const->std::size_t;

        [[nodiscard]]
        auto GetEntityInfo(const std::string& name) const -> const EntityInfo&;

        [[nodiscard]]
        auto GetEntityInfo(const std::string& name) -> EntityInfo&;

        /// @brief Create an new entity
        /// @param name Name of the created entity, must be unique
        /// @param type Type of the created entity
        /// @return Entity info to the created entity
        auto CreateEntity(const std::string& name, const std::string& type) -> EntityInfo&;

        /// @brief Destroy entity
        /// @param name Name of the entity
        /// @return The version of recycled entity
        void DestroyEntity(const std::string& name);

        /// @brief Emplace a component to an entity
        /// @tparam ...Args Types of arguments
        /// @tparam Component Type of component
        /// @param name Name of the entity
        /// @param ...args Arguments for constructing component
        /// @return Emplaced component
        template<cComponent Component, typename ...Args>
        auto EmplaceComponent(const std::string& name, Args&& ...args) -> Component&
        {
            return _Registry.emplace<Component>(_GetEntity(name), std::forward<Args>(args)...);
        }

        /// @brief Replace component in the entity
        /// @tparam ...Args Types of arguments
        /// @tparam Component Type of component
        /// @param name Name of the entity
        /// @param ...args Arguments for constructing component
        /// @return Replaced component
        template<cComponent Component, typename ...Args>
        auto ReplaceComponent(const std::string& name, Args&& ...args) -> Component&
        {
            ACDA_ASSERT(ContainsAllComponents<Component>(name));

            return _Registry.replace<Component>(_GetEntity(name), std::forward<Args>(args)...);
        }

        /// @brief Emplace or replace component in the entity
        /// @tparam ...Args Types of arguments
        /// @tparam Component Type of component
        /// @param name Name of the entity
        /// @param ...args Arguments for constructing component
        /// @return Emplaced/replaced component
        template<cComponent Component, typename ...Args>
        auto EmplaceOrReplaceComponent(const std::string& name, Args&& ...args) -> Component&
        {
            return _Registry.emplace_or_replace<Component>(_GetEntity(name), std::forward<Args>(args)...);
        }

        /// @brief Get component in the entity
        /// @tparam ...Components Types of component
        /// @param name Name of the entity
        /// @return Got component(s)
        template<cComponent ...Components>
        [[nodiscard]]
        auto GetComponent(const std::string& name) const -> decltype(auto)
        {
            ACDA_ASSERT(ContainsAllComponents<Components...>(name));

            return _Registry.get<Components...>(_GetEntity(name));
        }

        /// @copydoc Scene::Get
        template<cComponent ...Components>
        [[nodiscard]]
        auto GetComponent(const std::string& name) -> decltype(auto)
        {
            ACDA_ASSERT(ContainsAllComponents<Components...>(name));

            return _Registry.get<Components...>(_GetEntity(name));
        }

        template<cComponent ...Components>
        [[nodiscard]]
        auto ContainsAllComponents(const std::string& name) const -> bool
        {
            return _Registry.all_of<Components...>(_GetEntity(name));
        }

        template<cComponent ...Components>
        [[nodiscard]]
        auto ContainsAnyComponent(const std::string& name) const -> bool
        {
            return _Registry.any_of<Components...>(_GetEntity(name));
        }

        /// @brief Remove component from entity
        /// @tparam ...Component Type of component
        /// @param name Name of the entity
        template<cComponent ...Component>
        auto RemoveComponent(const std::string& name) -> RegistryType::size_type
        {
            auto count = _Registry.remove<Component...>(_GetEntity(name));
            return count;
        }

        /// @brief Get a view for the components
        /// @tparam ...Components Types of component used to construct the view
        /// @tparam ...ExcludeComponents Types of component used to filter the view
        /// @param exclude Helper class to specify @ref ...ExcludeComponents
        /// @return Created view
        template<cComponent ...Components, cComponent ...ExcludeComponents>
        [[nodiscard]]
        auto GetComponentView(entt::exclude_t<ExcludeComponents...> exclude = entt::exclude_t{}) -> decltype(auto)
        {
            auto view = _Registry.view<Components...>(exclude);
            return view;
        }

        /// @copydoc scene::view
        template<cComponent ...Components, cComponent ...ExcludeComponents>
        [[nodiscard]]
        auto GetComponentView(entt::exclude_t<ExcludeComponents...> exclude= entt::exclude_t{}) const -> decltype(auto)
        {
            return _Registry.view<Components...>(exclude);
        }

        /// @brief Get a group for the components
        /// @tparam ...OwnedComponents Types of component owned by the group
        /// @tparam ...GetComponents Types of component observed by the group, if any
        /// @tparam ...ExcludeComponents Types of component used to filter the group, if any
        /// @param get Helper class to specify @ref ...GetComponents
        /// @param exclude Helper class to specify @ref ...ExcludeComponents
        /// @return
        template<cComponent ...OwnedComponents, cComponent ...GetComponents, cComponent ...ExcludeComponents>
        [[nodiscard]]
        auto GetComponetGroup(entt::get_t<GetComponents...> get = entt::get_t{}, entt::exclude_t<ExcludeComponents...> exclude= entt::exclude_t{}) -> decltype(auto)
        {
            auto group = _Registry.group<OwnedComponents...>(get, exclude);
            return group;
        }

        [[nodiscard]]
        auto begin() const noexcept -> EntityInfoStorageType::const_iterator
        {
            return _EntityInfoStorage.begin();
        }
        [[nodiscard]]
        auto end() const noexcept -> EntityInfoStorageType::const_iterator
        {
            return _EntityInfoStorage.end();
        }
        [[nodiscard]]
        auto begin() noexcept -> EntityInfoStorageType::iterator
        {
            return _EntityInfoStorage.begin();
        }
        [[nodiscard]]
        auto end() noexcept -> EntityInfoStorageType::iterator
        {
            return _EntityInfoStorage.end();
        }

    private:
        auto _GetEntity(const std::string& name) const->entt::entity;
        auto _CreateJsonComponents(const std::string& name) const->nlohmann::json;
    public:
        std::string Name;
    private:

        entt::registry _Registry{};

        EntityInfoStorageType _EntityInfoStorage{};
    };
}