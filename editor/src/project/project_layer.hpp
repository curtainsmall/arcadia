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

struct ProjectLayer: iLayer
{
public:
    ProjectLayer();
    virtual ~ProjectLayer();

    virtual void OnEvent(EventBase& event) override;
    virtual void OnUpdate() override;

    [[nodiscard]]
    inline auto HasProject() const noexcept -> bool
    {
        return !!_Project;
    }
private:
    [[nodiscard]]
    auto _AssertAndGetScene() -> Scene&;

    void _SaveProject();
    void _LoadProject();

    void _OnWindowShouldClose(Event::WindowShouldClose& e);

    void _OnCreateProject(Event::CreateProject& e);
    void _OnOpenProject(Event::OpenProject& e);
    void _OnSaveProject(Event::SaveProject& e);
    void _OnSaveProjectAs(Event::SaveProjectAs& e);
    void _OnCloseProject(Event::CloseProject& e);
    void _OnProjectSaved(Event::ProjectSaved& e);

    void _OnCreateScene(Event::CreateScene& e);
    void _OnSelectScene(Event::SelectScene& e);
    void _OnCloseScene(Event::CloseScene& e);
    void _OnDeleteScene(Event::DeleteScene& e);

    void _OnNewEntity(Event::NewEntity& e);
    void _OnRenameEntity(Event::RenameEntity& e);
    void _OnDeleteEntity(Event::DeleteEntity& e);

    void _OnAddComponent(Event::AddComponent& e);
    void _OnRemoveComponent(Event::RemoveComponent& e);

private:
    std::filesystem::path _ProjectFilepath{};
    std::shared_ptr<Project> _Project{};

    std::shared_ptr<iRenderer> _Renderer{};
    std::shared_ptr<PhysicsSimulator> _PhysicsSimulator{};
};
