#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "core/event.hpp"
#include "core/exception.hpp"
#include "core/layer.hpp"
#include "core/enum.hpp"
#include "resource/scene.hpp"
#include "resource/scene_events.hpp"

namespace Arcadia
{
    namespace Exceptions
    {
        ACDA_DEFINE_EXCEPTION(CreatingSceneFromJsonFailed);
    }

    struct ACDA_API SceneLayer: public LayerInterface
    {
    public:
        enum class ActiveSceneModificationFlag: std::uint8_t
        {
            None = 0,
            Content = 0x01,
            Name = 0x02,
            Self = 0x04, // Whether another scene is set as active scene

            All = std::numeric_limits<std::underlying_type_t<ActiveSceneModificationFlag>>::max(),

            _EnumBitfield
        };

        using SceneStorageType = std::unordered_map<std::string, std::shared_ptr<Scene>>;
        using SelfType = SceneLayer;
    public:
        SceneLayer();
        virtual ~SceneLayer() override = default;

        virtual void OnEvent(EventBase& event);
        virtual void OnUpdate();

        [[nodiscard]]
        auto GetActiveSceneShared() const -> const std::shared_ptr<Scene>&;

        [[nodiscard]]
        auto HasScene() const -> bool;
        [[nodiscard]]
        auto HasScene(std::string_view name) const -> bool;
        [[nodiscard]]
        auto HasActiveScene() const -> bool;

        [[nodiscard]]
        auto GetSceneStorage() const -> const SceneStorageType&;

        [[nodiscard]]
        auto ActiveScene_GetName() const -> std::string_view;

        [[nodiscard]]
        auto ActiveScene_ContainsEntity(EntityId entity_id) const -> bool;

        [[nodiscard]]
        auto ActiveScene_IsEntityNameUsed(std::string_view entity_name) const -> bool;

        [[nodiscard]]
        auto ActiveScene_GetEntityIdByName(std::string_view entity_name) const -> EntityId;

        [[nodiscard]]
        auto ActiveScene_GetEntityCount() const -> std::size_t;

        [[nodiscard]]
        auto ActiveScene_GetEntityCount(const std::function<bool(EntityId, const EntityInfo&)>& pred) const -> std::size_t;

        [[nodiscard]]
        auto ActiveScene_GetEntityInfo(EntityId entity_id) const -> const EntityInfo&;

        [[nodiscard]]
        auto ActiveScene_GetEntityInfoStorage() const -> const Scene::EntityInfoStorageType&;

        template<Concepts::Component ...Components>
        [[nodiscard]]
        auto ActiveScene_ContainsAllComponents(EntityId entity_id) const -> bool
        {
            ACDA_ASSERT(_spActiveScene);
            return _spActiveScene->ContainsAllComponents<Components...>(entity_id);
        }

        template<Concepts::Component ...Components>
        [[nodiscard]]
        auto ActiveScene_CotainsAnyComponent(EntityId entity_id) const -> bool
        {
            ACDA_ASSERT(_spActiveScene);
            return _spActiveScene->ContainsAnyComponent<Components...>(entity_id);
        }

        template<Concepts::Component ...Components>
        [[nodiscard]]
        auto ActiveScene_GetComponent(EntityId entity_id) const -> decltype(auto)
        {
            ACDA_ASSERT(ActiveScene_ContainsAllComponents<Components...>(entity_id));
            return _spActiveScene->GetComponent<Components...>(entity_id);
        }

        [[nodiscard]]
        auto IsActiveSceneModified(ActiveSceneModificationFlag type) const -> bool;
        void MarkActiveSceneModified(ActiveSceneModificationFlag type, bool modified);

        void Snapshot();
        void Restore();

    private:
        void _SetActiveScene(std::string_view name = {}, bool not_considered_modified = false);

        void _CreateScene(std::string_view name);
        void _CreateScene(const nlohmann::json& json);
        auto _SaveScene(std::string_view name) -> nlohmann::json;
        void _DestroyScene(std::string_view name);
        void _RenameScene(std::string_view name, std::string_view new_name);

        void _OnCreateScene(Events::CreateScene& e);
        void _OnCreateSceneFromJson(Events::CreateSceneFromJson& e);
        void _OnRenameScene(Events::RenameScene& e);
        void _OnSelectScene(Events::SelectScene& e);
        void _OnCloseScene(Events::CloseScene& e);
        void _OnDeleteScene(Events::DeleteScene& e);
        void _OnDestroyAllScene(Events::DestroyAllScenes& e);

        void _OnNewEntity(Events::NewEntity& e);
        void _OnRenameEntity(Events::RenameEntity& e);
        void _OnDeleteEntity(Events::DeleteEntity& e);
        void _OnUpdateEntityInfo(Events::UpdateEntityInfo& e);

        void _OnAddComponent(Events::AddComponent& e);
        void _OnRemoveComponent(Events::RemoveComponent& e);

    private:
        SceneStorageType _SceneStorage{};
        std::shared_ptr<Scene> _spActiveScene{};
        ActiveSceneModificationFlag _ActiveSceneModificationFlag{ ActiveSceneModificationFlag::None };
    };
}