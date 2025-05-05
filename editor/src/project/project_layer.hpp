#pragma once

#include <memory>

#include "core/event.hpp"
#include "core/file.hpp"
#include "core/layer.hpp"
#include "function/physics/physics_simulator.hpp"
#include "function/render/renderer.hpp"
#include "function/window/window_events.hpp"
#include "platform/api_def.hpp"

#include "project/project.hpp"
#include "project/project_events.hpp"

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
        auto HasProject() const -> bool;

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

    private:
        std::filesystem::path _ProjectFilepath{};
        std::shared_ptr<Project> _spProject{};
        bool _ProjectModified{ false };
    };
}