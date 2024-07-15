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

    void _on_window_should_close(events::WindowShouldClose& e);

    void _on_create_project(events::CreateProject& e);
    void _on_open_project(events::OpenProject& e);
    void _on_save_project(events::SaveProject& e);
    void _on_save_project_as(events::SaveProjectAs& e);
    void _on_close_project(events::CloseProject& e);
    void _on_project_saved(events::ProjectSaved& e);

    void _on_create_scene(events::CreateScene& e);
    void _on_select_scene(events::SelectScene& e);
    void _on_close_scene(events::CloseScene& e);
    void _on_delete_scene(events::DeleteScene& e);

    void _on_new_entity(events::NewEntity& e);
    void _on_rename_entity(events::RenameEntity& e);
    void _on_delete_entity(events::DeleteEntity& e);

    void _on_add_component(events::AddComponent& e);
    void _on_remove_component(events::RemoveComponent& e);

private:
    std::filesystem::path _project_component{};
    std::shared_ptr<Project> _project{};

    std::shared_ptr<iRenderer> _renderer{};
    std::shared_ptr<PhysicsSimulator> _physics_simulator{};
};
