#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/file/file.hpp"
#include"core/layer/layer.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/render/renderer.hpp"
#include"function/window/window_events.hpp"

#include"project/project.hpp"
#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace Arcadia
{
    struct ProjectLayer: Arcadia::iLayer
    {
    public:
        ProjectLayer();
        virtual ~ProjectLayer();

        virtual void OnEvent(Arcadia::EventBase& event) override;
        virtual void OnUpdate() override;

        [[nodiscard]]
        inline auto HasProject() const noexcept -> bool
        {
            return !!_Project;
        }
    private:
        [[nodiscard]]
        auto _AssertAndGetScene() -> Arcadia::Scene&;

        void _SaveProject();
        void _LoadProject();

        void _OnWindowShouldClose(Arcadia::Event::WindowShouldClose& e);

        void _OnCreateProject(Arcadia::Event::CreateProject& e);
        void _OnOpenProject(Arcadia::Event::OpenProject& e);
        void _OnSaveProject(Arcadia::Event::SaveProject& e);
        void _OnSaveProjectAs(Arcadia::Event::SaveProjectAs& e);
        void _OnCloseProject(Arcadia::Event::CloseProject& e);
        void _OnProjectSaved(Arcadia::Event::ProjectSaved& e);

        void _OnCreateScene(Arcadia::Event::CreateScene& e);
        void _OnSelectScene(Arcadia::Event::SelectScene& e);
        void _OnCloseScene(Arcadia::Event::CloseScene& e);
        void _OnDeleteScene(Arcadia::Event::DeleteScene& e);

        void _OnNewEntity(Arcadia::Event::NewEntity& e);
        void _OnRenameEntity(Arcadia::Event::RenameEntity& e);
        void _OnDeleteEntity(Arcadia::Event::DeleteEntity& e);

        void _OnAddComponent(Arcadia::Event::AddComponent& e);
        void _OnRemoveComponent(Arcadia::Event::RemoveComponent& e);

    private:
        std::filesystem::path _ProjectFilepath{};
        std::shared_ptr<Arcadia::Project> _Project{};

        std::shared_ptr<Arcadia::iRenderer> _Renderer{};
        std::shared_ptr<Arcadia::PhysicsSimulator> _PhysicsSimulator{};
    };
}
