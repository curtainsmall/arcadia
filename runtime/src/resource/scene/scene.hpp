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

namespace Arcadia
{
    struct ARCADIA_API EntityInfo: Arcadia::Noncopyable
    {
    public:
        using self_type = EntityInfo;
    public:
        EntityInfo() = default;
        EntityInfo(const nlohmann::json& json);
        [[nodiscard]]
        auto ToJson() const->nlohmann::json;
    public:
        bool ShouldRenderInViewport{ true };
    };

    struct Scene;
    struct ARCADIA_API SceneMementoData
    {
        friend struct Arcadia::Scene;
    private:
        struct ARCADIA_API EmplacedEntityInfo
        {
        public:
            using self_type = EmplacedEntityInfo;
        public:
            std::string Name{};
        };
        struct ARCADIA_API ReplacedEntityInfo
        {
        public:
            using self_type = ReplacedEntityInfo;
        public:
            std::string PreviousName{};
            std::string CurrentName{};

        };

    private:
        std::string Name;
    };

    struct ARCADIA_API Scene: Arcadia::Noncopyable
    {
    public:
        using registry_type = entt::registry;
        using self_type = Scene;
    public:
        Scene(const std::string& name):
            _Name(name)
        {}
        Scene(const nlohmann::json& json);
        ~Scene() = default;
        auto ToJson() const->nlohmann::json;

        Scene(self_type&&) noexcept = default;
        auto operator=(self_type&&) noexcept -> self_type & = default;


        [[nodiscard]]
        auto GetName() const -> const std::string&;
        void SetName(const std::string& name);

        [[nodiscard]]
        auto GetRegistry() const -> const registry_type&
        {
            return _Registry;
        }

        [[nodiscard]]
        auto GetNameEntityBimap() const -> const boost::bimap<std::string, entt::entity>
        {
            return _NameEntityBimap;
        }
        [[nodiscard]]
        auto GetNameOfEntity(const entt::entity entity) const -> const std::string&;
        [[nodiscard]]
        auto GetEntityOfName(const std::string& name) const->entt::entity;

        [[nodiscard]]
        auto GetEntityInfo(const entt::entity entity) const -> const Arcadia::EntityInfo&;
        [[nodiscard]]
        auto GetEntityInfo(const entt::entity entity) -> Arcadia::EntityInfo&;

        /// @brief Rename entity
        /// @param old_name Old name
        /// @param new_name New name
        /// @return Whether succeeded
        auto Rename(const std::string& old_name, const std::string& new_name) -> bool;

        /// @brief Check whether there is an entity with given name
        /// @param name Name of entity
        /// @return Result
        [[nodiscard]]
        auto Contains(const std::string& name) const -> bool;

        /// @brief Create an new entity
        /// @param name Name of the entity
        /// @return Created entity
        [[nodiscard]]
        auto Create(const std::string& name) -> entt::entity;

        /// @brief Create an new entity
        /// @param name Name of the entity
        /// @param json_entity_info Json used to contruct @ref Arcadia::entity_info
        /// @return Created entity
        [[nodiscard]]
        auto Create(const std::string& name, const nlohmann::json& json_entity_info) -> entt::entity;

        /// @brief Destroy entity
        /// @param entity Entity to destroy
        /// @return The version of recycled entity
        auto Destroy(const entt::entity entity) -> registry_type::version_type;

        /// @brief Check whether entity is valid
        /// @param entity Entity to check
        /// @return Result
        [[nodiscard]]
        auto IsValid(const entt::entity entity) const -> bool;

        /// @brief Emplace a component to an entity
        /// @tparam ...Args Types of arguments
        /// @tparam Component Type of component
        /// @param entity Entity to emplace component to
        /// @param ...args Arguments for constructing component
        /// @return Emplaced component
        template<Arcadia::cComponent Component, class ...Args>
        auto Emplace(const entt::entity entity, Args&& ...args) -> Component&
        {
            auto& comp = _Registry.emplace<Component>(entity, std::forward<Args>(args)...);
            return comp;
        }

        /// @brief Replace component in the entity
        /// @tparam ...Args Types of arguments
        /// @tparam Component Type of component
        /// @param entity Entity to replace component to
        /// @param ...args Arguments for constructing component
        /// @return Replaced component
        template<Arcadia::cComponent Component, class ...Args>
        auto Replace(const entt::entity entity, Args&& ...args) -> Component&
        {
            ARCADIA_ASSERT(AllOf<Component>(entity));

            auto& comp = _Registry.replace<Component>(entity, std::forward<Args>(args)...);
            return comp;
        }

        /// @brief Emplace or replace component in the entity
        /// @tparam ...Args Types of arguments
        /// @tparam Component Type of component
        /// @param entity Entity to replace component to
        /// @param ...args Arguments for constructing component
        /// @return Emplaced/replaced component
        template<Arcadia::cComponent Component, class ...Args>
        auto EmplaceOrReplace(const entt::entity entity, Args&& ...args) -> Component&
        {
            auto& comp = _Registry.emplace_or_replace<Component>(entity, std::forward<Args>(args)...);
            return comp;
        }

        /// @brief Get component in the entity
        /// @tparam ...Components Types of component
        /// @param entity Entity to get component from
        /// @return Got component(s)
        template<Arcadia::cComponent ...Components>
        [[nodiscard]]
        auto Get(const entt::entity entity) const -> decltype(auto)
        {
            ARCADIA_ASSERT(AllOf<Components...>(entity));

            return _Registry.get<Components...>(entity);
        }

        /// @copydoc Arcadia::scece::get
        template<Arcadia::cComponent ...Components>
        [[nodiscard]]
        auto Get(const entt::entity entity) -> decltype(auto)
        {
            ARCADIA_ASSERT(AllOf<Components...>(entity));

            auto& comp = _Registry.get<Components...>(entity);
            return comp;
        }

        template<Arcadia::cComponent ...Components>
        [[nodiscard]]
        auto AllOf(const entt::entity entity) const -> bool
        {
            return _Registry.all_of<Components...>(entity);
        }

        template<Arcadia::cComponent ...Components>
        [[nodiscard]]
        auto AnyOf(const entt::entity entity) const -> bool
        {
            return _Registry.any_of<Components...>(entity);
        }

        /// @brief Remove component from entity
        /// @tparam ...Component Type of component
        /// @param entity Entity to remove component from
        template<Arcadia::cComponent ...Component>
        auto Remove(const entt::entity entity) -> registry_type::size_type
        {
            auto count = _Registry.remove<Component...>(entity);
            return count;
        }

        /// @brief Get a view for the components
        /// @tparam ...Components Types of component used to construct the view
        /// @tparam ...ExcludeComponents Types of component used to filter the view
        /// @param exclude Helper class to specify @ref ...ExcludeComponents
        /// @return Created view
        template<Arcadia::cComponent ...Components, Arcadia::cComponent ...ExcludeComponents>
        [[nodiscard]]
        auto View(entt::exclude_t<ExcludeComponents...> exclude = entt::exclude_t{}) -> decltype(auto)
        {
            auto view = _Registry.view<Components...>(exclude);
            return view;
        }

        /// @copydoc Arcadia::scene::view
        template<Arcadia::cComponent ...Components, Arcadia::cComponent ...ExcludeComponents>
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
        template<Arcadia::cComponent ...OwnedComponents, Arcadia::cComponent ...GetComponents, Arcadia::cComponent ...ExcludeComponents>
        [[nodiscard]]
        auto Group(entt::get_t<GetComponents...> get = entt::get_t{}, entt::exclude_t<ExcludeComponents...> exclude= entt::exclude_t{}) -> decltype(auto)
        {
            auto group = _Registry.group<OwnedComponents...>(get, exclude);
            return group;
        }

    private:
        auto _CreateJsonComponents(const entt::entity entity) const->nlohmann::json;
    private:
        std::string _Name;

        boost::bimap<std::string, entt::entity> _NameEntityBimap{};
        std::unordered_map<entt::entity, Arcadia::EntityInfo> _umapEntityInfo{};
        entt::registry _Registry{};
    };

    struct ARCADIA_API JsonSceneAddComponentToEntityHelper: Arcadia::Noncopyable
    {
    public:
        using self_type = JsonSceneAddComponentToEntityHelper;
    public:
        JsonSceneAddComponentToEntityHelper(const Arcadia::Scene& scene, nlohmann::json& json_entities):
            _pScene(&scene),
            _pJsonEntity(&json_entities)
        {}

        template<Arcadia::cComponent Component>
        auto Add(const std::string& Type) -> self_type&
        {
            for(auto [entity, comp] : _pScene->View<Component>().each())
            {
                _pJsonEntity
                    ->at(_pScene->GetNameOfEntity(entity))
                    .push_back({ Type , comp.ToJson() });
            }
            return *this;
        }
    private:
        const Arcadia::Scene* _pScene;
        nlohmann::json* _pJsonEntity;
    };
}
