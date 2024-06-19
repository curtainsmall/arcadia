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

    virtual void on_event(EventBase& e) override;
    virtual void on_update() override;

    [[nodiscard]]
    inline auto has_project() const noexcept -> bool
    {
        return !!_project;
    }
private:
    [[nodiscard]]
    auto _assert_and_get_scene() -> Scene&;

    void _save_project();
    void _load_project();

    void _on_window_should_close(event::WindowShouldClose& e);

    void _on_create_project(event::CreateProject& e);
    void _on_open_project(event::OpenProject& e);
    void _on_save_project(event::SaveProject& e);
    void _on_save_project_as(event::SaveProjectAs& e);
    void _on_close_project(event::CloseProject& e);
    void _on_project_saved(event::ProjectSaved& e);

    void _on_create_scene(event::CreateScene& e);
    void _on_select_scene(event::SelectScene& e);
    void _on_close_scene(event::CloseScene& e);
    void _on_delete_scene(event::DeleteScene& e);

    void _OnNewEntity(event::NewEntity& e);
    void _on_rename_entity(event::RenameEntity& e);
    void _on_delete_entity(event::DeleteEntity& e);

    void _on_add_component(event::AddComponent& e);
    void _on_remove_component(event::RemoveComponent& e);

private:
    std::filesystem::path _project_component{};
    std::shared_ptr<Project> _project{};

    std::shared_ptr<iRenderer> _renderer{};
    std::shared_ptr<PhysicsSimulator> _physics_simulator{};
};
