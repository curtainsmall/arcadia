#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include "core/assert.hpp"
#include "core/event/event.hpp"
#include "core/exception.hpp"
#include "core/nlohmann_json_header.hpp"
#include "core/noncopyable.hpp"
#include "core/string.hpp"
#include "core/uuid.hpp"
#include "platform/api_def.hpp"
#include "resource/components/component_interface.hpp"
#include "resource/entity_id.hpp"
#include "resource/entt_header.hpp"

namespace Arcadia
{
    static constexpr std::array BuildinEntityTypes{
         "actor",
         "camera",
         "light",
    };

    class EntityInfo
    {
        friend class Scene;
    public:
        using SelfType = EntityInfo;

    public:
        [[nodiscard]]
        inline auto GetName() const -> const std::string&
        {
            return _Name;
        }
    public:
        std::string TypeString{}; // Type of the entity
        bool Displayed{ true }; // Whether the entity will be displayed in the viewport (the renderer will skip the hidden ones)
        bool Internal{ false }; // Whether the entity is controled internally (it will not be listed in the outliner); Note that an internal entity will still be rendered unless `Display` is set to false
    private:
        std::string _Name;
    };

    class Scene: public Noncopyable
    {
    public:
        using EntityInfoStorageType = std::unordered_map<EntityId, EntityInfo>;
        using EntityNameToEntityIdLookupMap = std::unordered_map<std::string, EntityId>;
        using RegistryType = entt::registry;
        using SelfType = Scene;
    public:
        Scene(const std::string& name):
            _Name(name)
        {}
        Scene(const nlohmann::json& json);
        ~Scene() = default;
        auto ToJson() const->nlohmann::json;

        Scene(SelfType&&) noexcept = default;
        auto operator=(SelfType&&) noexcept -> SelfType & = default;

        [[nodiscard]]
        auto GetName() const -> const std::string&;
        void SetName(const std::string& name);

        [[nodiscard]]
        auto ContainsEntity(EntityId entity_id) const -> bool;

        [[nodiscard]]
        auto GetSize() const->std::size_t;

        [[nodiscard]]
        auto CountEntity(const std::function<bool(EntityId, const EntityInfo&)>& pred) const->std::size_t;

        [[nodiscard]]
        auto GetEntityInfo(EntityId entity_id) const -> const EntityInfo&;

        [[nodiscard]]
        auto GetEntityInfo(EntityId entity_id) -> EntityInfo&;

        [[nodiscard]]
        auto CreateEntity(const std::string& entity_name, const std::string& type_string) -> EntityId;

        void DestroyEntity(EntityId entity_id);

        void RenameEntity(EntityId entity_id, const std::string& new_entity_name);

        template<Concepts::Component Component, typename ...Args>
        auto EmplaceComponent(EntityId entity_id, Args&& ...args) -> Component&
        {
            return _Registry.emplace<Component>(entity_id, std::forward<Args>(args)...);
        }

        template<Concepts::Component Component, typename ...Args>
        auto ReplaceComponent(EntityId entity_id, Args&& ...args) -> Component&
        {
            ACDA_ASSERT(ContainsAllComponents<Component>(entity_id));

            return _Registry.replace<Component>(entity_id, std::forward<Args>(args)...);
        }

        template<Concepts::Component Component, typename ...Args>
        auto EmplaceOrReplaceComponent(EntityId entity_id, Args&& ...args) -> Component&
        {
            return _Registry.emplace_or_replace<Component>(entity_id, std::forward<Args>(args)...);
        }

        template<Concepts::Component ...Components>
        [[nodiscard]]
        auto GetComponent(EntityId entity_id) const -> decltype(auto)
        {
            ACDA_ASSERT(ContainsAllComponents<Components...>(entity_id));

            return _Registry.get<Components...>(entity_id);
        }

        template<Concepts::Component ...Components>
        [[nodiscard]]
        auto GetComponent(EntityId entity_id) -> decltype(auto)
        {
            ACDA_ASSERT(ContainsAllComponents<Components...>(entity_id));

            return _Registry.get<Components...>(entity_id);
        }

        template<Concepts::Component ...Components>
        [[nodiscard]]
        auto ContainsAllComponents(EntityId entity_id) const -> bool
        {
            return _Registry.all_of<Components...>(entity_id);
        }

        template<Concepts::Component ...Components>
        [[nodiscard]]
        auto ContainsAnyComponent(EntityId entity_id) const -> bool
        {
            return _Registry.any_of<Components...>(entity_id);
        }

        template<Concepts::Component ...Component>
        auto RemoveComponent(EntityId entity_id) -> RegistryType::size_type
        {
            auto count = _Registry.remove<Component...>(entity_id);
            return count;
        }

        template<Concepts::Component ...Components, Concepts::Component ...ExcludeComponents>
        [[nodiscard]]
        auto GetComponentView(entt::exclude_t<ExcludeComponents...> exclude = entt::exclude_t{}) -> decltype(auto)
        {
            auto view = _Registry.view<Components...>(exclude);
            return view;
        }

        template<Concepts::Component ...Components, Concepts::Component ...ExcludeComponents>
        [[nodiscard]]
        auto GetComponentView(entt::exclude_t<ExcludeComponents...> exclude= entt::exclude_t{}) const -> decltype(auto)
        {
            return _Registry.view<Components...>(exclude);
        }

        template<Concepts::Component ...OwnedComponents, Concepts::Component ...GetComponents, Concepts::Component ...ExcludeComponents>
        [[nodiscard]]
        auto GetComponetGroup(entt::get_t<GetComponents...> get = entt::get_t{}, entt::exclude_t<ExcludeComponents...> exclude= entt::exclude_t{}) -> decltype(auto)
        {
            auto group = _Registry.group<OwnedComponents...>(get, exclude);
            return group;
        }

        [[nodiscard]]
        auto GetEntityInfoStorage() -> EntityInfoStorageType&
        {
            return _EntityInfoStorage;
        }

        [[nodiscard]]
        auto IsEntityNameUsed(const std::string& entity_name) const -> bool;

        [[nodiscard]]
        auto GetEntityIdByName(const std::string& entity_name) const->EntityId;

    private:
        auto _CreateJsonComponents(EntityId entity_id) const->nlohmann::json;
    private:
        std::string _Name;

        entt::registry _Registry{};

        EntityInfoStorageType _EntityInfoStorage{};
        EntityNameToEntityIdLookupMap _EntityNameToEntityIdLookupMap{};
    };
}