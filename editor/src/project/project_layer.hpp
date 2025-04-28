#pragma once

#include<memory>

#include"core/event/event.hpp"
#include"core/file/file.hpp"
#include"core/layer/layer.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/render/renderer.hpp"
#include"function/window/window_events.hpp"
#include"platform/api_def.hpp"

#include"project/project.hpp"
#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace Arcadia
{
    class ProjectLayer: public LayerInterface
    {
    public:
        ProjectLayer();
        virtual ~ProjectLayer();

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;

        [[nodiscard]]
        inline auto HasProject() const noexcept -> bool
        {
            return !!_spProject;
        }
    private:
        void _SaveProject();
        void _LoadProject();

        void _OnWindowShouldClose(Events::WindowShouldClose& e);

        void _OnCreateProject(Events::CreateProject& e);
        void _OnOpenProject(Events::OpenProject& e);
        void _OnSaveProject(Events::SaveProject& e);
        void _OnSaveProjectAs(Events::SaveProjectAs& e);
        void _OnCloseProject(Events::CloseProject& e);
        void _OnProjectSaved(Events::ProjectSaved& e);

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
        std::filesystem::path _ProjectFilepath{};
        std::shared_ptr<Project> _spProject{};
        bool _ProjectModified{ false };

        std::shared_ptr<RendererInterface> _spRenderer{};
        std::shared_ptr<PhysicsSimulator> _spPhysicsSimulator{};
    };
}