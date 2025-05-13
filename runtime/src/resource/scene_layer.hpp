#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "core/layer.hpp"
#include "core/event.hpp"
#include "core/exception.hpp"
#include "resource/scene.hpp"
#include "resource/scene_events.hpp"

namespace Arcadia
{
    namespace Exceptions
    {
        ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION(CreatingSceneFromJsonFailed);
    }

    class SceneLayer: public LayerInterface
    {
    public:
        using SceneStorageType = std::unordered_map<std::string, std::shared_ptr<Scene>>;
        using SelfType = SceneLayer;
    public:
        SceneLayer();
        virtual ~SceneLayer();

        virtual void OnEvent(EventBase& event);
        virtual void OnUpdate();

        [[nodiscard]]
        auto GetActiveSceneShared() -> std::shared_ptr<Scene>&;
        [[nodiscard]]
        auto GetActiveSceneShared() const -> const std::shared_ptr<Scene>&;
        void SetActiveScene(const std::string& name = {});

        [[nodiscard]]
        auto HasScene() const -> bool;
        [[nodiscard]]
        auto HasScene(const std::string& name) const -> bool;
        [[nodiscard]]
        auto HasActiveScene() const -> bool;
        void CreateScene(const std::string& name);
        void CreateScene(const nlohmann::json& json);
        auto SaveScene(const std::string& name) -> nlohmann::json;
        void DestroyScene(const std::string& name);
        void DestroyAllScenes();
        void RenameScene(const std::string& name, const std::string& new_name);

        [[nodiscard]]
        auto GetSceneStorage() const -> const SceneStorageType&;

        [[nodiscard]]
        auto ActiveScene_GetName() const -> const std::string&;

        [[nodiscard]]
        auto ActiveScene_ContainsEntity(EntityId entity_id) const -> bool;

        [[nodiscard]]
        auto ActiveScene_IsEntityNameUsed(const std::string& entity_name) const -> bool;

        [[nodiscard]]
        auto ActiveScene_GetEntityIdByName(const std::string& entity_name) const->EntityId;

        [[nodiscard]]
        auto ActiveScene_GetEntityCount() const->std::size_t;

        [[nodiscard]]
        auto ActiveScene_GetEntityCount(const std::function<bool(EntityId, const EntityInfo&)>& pred) const->std::size_t;

        [[nodiscard]]
        auto ActiveScene_GetEntityInfo(EntityId entity_id) const -> const EntityInfo&;

        [[nodiscard]]
        auto ActiveScene_GetEntityInfoStorage() const -> const Scene::EntityInfoStorageType&;

        template<Concepts::Component ...Ts_Components>
        [[nodiscard]]
        auto ActiveScene_ContainsAllComponents(EntityId entity_id) const -> bool
        {
            return _spActiveScene->ContainsAllComponents<Ts_Components...>(entity_id);
        }

        template<Concepts::Component ...Ts_Components>
        [[nodiscard]]
        auto ActiveScene_CotainsAnyComponent(EntityId entity_id) const -> bool
        {
            return _spActiveScene->ContainsAnyComponent<Ts_Components...>(entity_id);
        }

        template<Concepts::Component ...Ts_Components>
        [[nodiscard]]
        auto ActiveScene_GetComponent(EntityId entity_id) const -> decltype(auto)
        {
            ACDA_ASSERT(ActiveScene_ContainsAllComponents<Ts_Components...>(entity_id));
            return _spActiveScene->GetComponent<Ts_Components...>(entity_id);
        }

        [[nodiscard]]
        auto IsActiveSceneModified() const -> bool;

    private:
        void _OnCreateScene(Events::CreateScene& e);
        void _OnRenameScene(Events::RenameScene& e);
        void _OnSelectScene(Events::SelectScene& e);
        void _OnCloseScene(Events::CloseScene& e);
        void _OnDeleteScene(Events::DeleteScene& e);

        void _OnNewEntity(Events::NewEntity& e);
        void _OnRenameEntity(Events::RenameEntity& e);
        void _OnDeleteEntity(Events::DeleteEntity& e);
        void _OnUpdateEntityInfo(Events::UpdateEntityInfo& e);

        void _OnAddComponent(Events::AddComponent& e);
        void _OnRemoveComponent(Events::RemoveComponent& e);

    private:
        SceneStorageType _SceneStorage{};
        std::shared_ptr<Scene> _spActiveScene{};
        bool _ActiveSceneModified{ false };
    };
}
