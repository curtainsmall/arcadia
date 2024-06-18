#pragma once

#include<filesystem>
#include<string>
#include<unordered_map>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/exception.hpp"
#include"core/memento/memento.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/string.hpp"
#include"core/uuid.hpp"
#include"resource/components/component_interface.hpp"
#include"resource/entt_header.hpp"

static inline std::array BuildInEntityTypes{
     "actor"s,
     "camera"s,
     "light"s,
};

struct EntityInfo
{
    friend struct Scene;
public:
    using self_type = EntityInfo;
public:
    EntityInfo(
        const std::string& name,
        const entt::entity entity
    ):
        Name(name),
        Entity(entity)
    {}

    [[nodiscard]]
    auto GetName() const -> const std::string&;

    [[nodiscard]]
    auto GetEntity() const->entt::entity;

public:
    std::string Type{}; // Type of the entity
    bool Display{ true }; // Whether the entity will be displayed in the viewport (the renderer will skip the hidden ones)
    bool Internal{ false }; // Whether the entity is controled internally (it will not be listed in the outliner); Note that an internal entity will still be rendered unless `Display` is set to false
protected:
    std::string Name{};
    entt::entity Entity{};
};

struct Scene: Noncopyable
{
public:
    using entity_info_storage_type = std::unordered_map<std::string, EntityInfo>;
    using registry_type = entt::registry;
    using self_type = Scene;
public:
    Scene(const std::string& name):
        Name(name)
    {}
    Scene(const nlohmann::json& json);
    ~Scene() = default;
    auto ToJson() const->nlohmann::json;

    Scene(self_type&&) noexcept = default;
    auto operator=(self_type&&) noexcept -> self_type & = default;

    [[nodiscard]]
    auto GetRegistry() const -> const registry_type&
    {
        return _Registry;
    }

    /// @brief Set name of entity
    /// @param entity Entity to set name
    /// @param name Name
    /// @note Do not use this function when iterating entities
    void Rename(const std::string& name, const std::string& new_name);

    /// @brief Check whether there is an entity with given name
    /// @param name Name of entity
    /// @return Result
    [[nodiscard]]
    auto Contains(const std::string& name) const -> bool;

    [[nodiscard]]
    auto Size() const->std::size_t;

    [[nodiscard]]
    auto Count(const std::function<bool(const std::string&, const EntityInfo&)>& pred) const->std::size_t;

    [[nodiscard]]
    auto GetEntityInfo(const std::string& name) const -> const EntityInfo&;

    [[nodiscard]]
    auto GetEntityInfo(const std::string& name) -> EntityInfo&;

    /// @brief Create an new entity
    /// @param name Name of the created entity, must be unique
    /// @param type Type of the created entity
    /// @return Entity info to the created entity
    auto Create(const std::string& name, const std::string& type) -> EntityInfo&;

    /// @brief Destroy entity
    /// @param name Name of the entity
    /// @return The version of recycled entity
    void Destroy(const std::string& name);

    /// @brief Emplace a component to an entity
    /// @tparam ...Args Types of arguments
    /// @tparam Component Type of component
    /// @param name Name of the entity
    /// @param ...args Arguments for constructing component
    /// @return Emplaced component
    template<cComponent Component, class ...Args>
    auto Emplace(const std::string& name, Args&& ...args) -> Component&
    {
        return _Registry.emplace<Component>(_EntityOf(name), std::forward<Args>(args)...);
    }

    /// @brief Replace component in the entity
    /// @tparam ...Args Types of arguments
    /// @tparam Component Type of component
    /// @param name Name of the entity
    /// @param ...args Arguments for constructing component
    /// @return Replaced component
    template<cComponent Component, class ...Args>
    auto Replace(const std::string& name, Args&& ...args) -> Component&
    {
        ACDA_ASSERT(AllOf<Component>(name));

        return _Registry.replace<Component>(_EntityOf(name), std::forward<Args>(args)...);
    }

    /// @brief Emplace or replace component in the entity
    /// @tparam ...Args Types of arguments
    /// @tparam Component Type of component
    /// @param name Name of the entity
    /// @param ...args Arguments for constructing component
    /// @return Emplaced/replaced component
    template<cComponent Component, class ...Args>
    auto EmplaceOrReplace(const std::string& name, Args&& ...args) -> Component&
    {
        return _Registry.emplace_or_replace<Component>(_EntityOf(name), std::forward<Args>(args)...);
    }

    /// @brief Get component in the entity
    /// @tparam ...Components Types of component
    /// @param name Name of the entity
    /// @return Got component(s)
    template<cComponent ...Components>
    [[nodiscard]]
    auto Get(const std::string& name) const -> decltype(auto)
    {
        ACDA_ASSERT(AllOf<Components...>(name));

        return _Registry.get<Components...>(_EntityOf(name));
    }

    /// @copydoc Scene::Get
    template<cComponent ...Components>
    [[nodiscard]]
    auto Get(const std::string& name) -> decltype(auto)
    {
        ACDA_ASSERT(AllOf<Components...>(name));

        return _Registry.get<Components...>(_EntityOf(name));
    }

    template<cComponent ...Components>
    [[nodiscard]]
    auto AllOf(const std::string& name) const -> bool
    {
        return _Registry.all_of<Components...>(_EntityOf(name));
    }

    template<cComponent ...Components>
    [[nodiscard]]
    auto AnyOf(const std::string& name) const -> bool
    {
        return _Registry.any_of<Components...>(_EntityOf(name));
    }

    /// @brief Remove component from entity
    /// @tparam ...Component Type of component
    /// @param name Name of the entity
    template<cComponent ...Component>
    auto Remove(const std::string& name) -> registry_type::size_type
    {
        auto count = _Registry.remove<Component...>(_EntityOf(name));
        return count;
    }

    /// @brief Get a view for the components
    /// @tparam ...Components Types of component used to construct the view
    /// @tparam ...ExcludeComponents Types of component used to filter the view
    /// @param exclude Helper class to specify @ref ...ExcludeComponents
    /// @return Created view
    template<cComponent ...Components, cComponent ...ExcludeComponents>
    [[nodiscard]]
    auto View(entt::exclude_t<ExcludeComponents...> exclude = entt::exclude_t{}) -> decltype(auto)
    {
        auto view = _Registry.view<Components...>(exclude);
        return view;
    }

    /// @copydoc scene::view
    template<cComponent ...Components, cComponent ...ExcludeComponents>
    [[nodiscard]]
    auto View(entt::exclude_t<ExcludeComponents...> exclude= entt::exclude_t{}) const -> decltype(auto)
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
    auto Group(entt::get_t<GetComponents...> get = entt::get_t{}, entt::exclude_t<ExcludeComponents...> exclude= entt::exclude_t{}) -> decltype(auto)
    {
        auto group = _Registry.group<OwnedComponents...>(get, exclude);
        return group;
    }

    [[nodiscard]]
    auto begin() const noexcept -> entity_info_storage_type::const_iterator
    {
        return _EntityInfoStorage.begin();
    }
    [[nodiscard]]
    auto end() const noexcept -> entity_info_storage_type::const_iterator
    {
        return _EntityInfoStorage.end();
    }
    [[nodiscard]]
    auto begin() noexcept -> entity_info_storage_type::iterator
    {
        return _EntityInfoStorage.begin();
    }
    [[nodiscard]]
    auto end() noexcept -> entity_info_storage_type::iterator
    {
        return _EntityInfoStorage.end();
    }

private:
    auto _EntityOf(const std::string& name) const->entt::entity;
    auto _CreateJsonComponents(const std::string& name) const->nlohmann::json;
public:
    std::string Name;
private:

    entt::registry _Registry{};

    entity_info_storage_type _EntityInfoStorage{};
};
