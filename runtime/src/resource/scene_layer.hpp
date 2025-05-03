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
        auto HasActiveScene() const -> bool;
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
        auto GetSceneShared(const std::string& name) -> std::shared_ptr<Scene>&;
        [[nodiscard]]
        auto GetSceneShared(const std::string& name) const -> const std::shared_ptr<Scene>&;
        void CreateScene(const std::string& name);
        void CreateScene(const nlohmann::json& json);
        auto SaveScene(const std::string& name) -> nlohmann::json;
        void DestroyScene(const std::string& name);
        void DestroyAllScenes();
        void RenameScene(const std::string& name, const std::string& new_name);

        [[nodiscard]]
        auto GetSceneStorage() const -> const SceneStorageType&;

        [[nodiscard]]
        auto IsSceneModified() const -> bool;

    private:
        void _OnCreateScene(Events::CreateScene& e);
        void _OnRenameScene(Events::RenameScene& e);
        void _OnSelectScene(Events::SelectScene& e);
        void _OnCloseScene(Events::CloseScene& e);
        void _OnDeleteScene(Events::DeleteScene& e);

        void _OnNewEntity(Events::NewEntity& e);
        void _OnRenameEntity(Events::RenameEntity& e);
        void _OnDeleteEntity(Events::DeleteEntity& e);

        void _OnAddComponent(Events::AddComponent& e);
        void _OnRemoveComponent(Events::RemoveComponent& e);

    private:
        SceneStorageType _SceneStorage{};
        std::shared_ptr<Scene> _spActiveScene{};
        bool _SceneModified{ false };
    };
}
