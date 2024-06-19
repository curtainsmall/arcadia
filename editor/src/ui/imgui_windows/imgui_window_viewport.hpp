#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/input/input_events.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/render/renderer.hpp"
#include"ui/imgui_header.hpp"
#include"resource/scene.hpp"

#include"ui/imgui_window.hpp"
#include"project/project.hpp"
#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

struct ImguiWindowViewport: iImguiWindow
{
public:
    enum struct GizmoOption
    {
        None = 0,
        Translation = ImGuizmo::OPERATION::TRANSLATE,
        Rotation = ImGuizmo::OPERATION::ROTATE,
        Scale = ImGuizmo::OPERATION::SCALE,
    };
    enum struct GizmoMode
    {
        Local = ImGuizmo::MODE::LOCAL,
        World = ImGuizmo::MODE::WORLD,
    };
    using self_type = ImguiWindowViewport;
public:
    ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###viewport");

    inline ImguiWindowViewport(
        bool open,
        const std::string& title
    ):
        iImguiWindow(open, title)
    {}
    virtual ~ImguiWindowViewport() = default;

    virtual void on_event(EventBase& e) override;
    virtual void on_update() override;

private:
    void _on_input_cursor_move(event::InputCursorMove& e);
    void _on_open_imgui_window(event::OpenImguiWindow& e);
    void _on_project_built(event::ProjectBuilt& e);
    void _on_project_unbuilt(event::ProjectUnbuilt& e);
    void _on_scene_activated(event::SceneActivated& e);
    void _on_scene_deactivated(event::SceneDeactivated& e);
    void _on_select_entity(event::SelectEntity& e);
    void _on_rename_entity(event::RenameEntity& e);
    void _on_delete_entity(event::DeleteEntity& e);
    void _on_renderer_built(event::RendererBuilt& e);
    void _on_renderer_unbuilt(event::RendererUnbuilt& e);
    void _on_physics_simulator_built(event::PhysicsSimulatorBuilt& e);
    void _on_physics_simulator_unbuilt(event::PhysicsSimulatorUnbuilt& e);
    void _on_show_gizmo(event::ShowGizmo& e);

public:
    static inline std::string viewport_camera_entity_name{ "viewport_camera" };
private:
    std::weak_ptr<Project> _project{};
    std::weak_ptr<Scene> _scene{};
    std::weak_ptr<iRenderer> _renderer{};
    std::weak_ptr<PhysicsSimulator> _physics_simulator{};

    std::string _selected_entity_name{};

    glm::vec2 _cursor_move{};

    bool _in_viewport_free_cam{ false };
    bool _show_gizmo{ false };

    GizmoOption _gizmo_option{ GizmoOption::None };
    GizmoMode _gizmo_mode{ GizmoMode::Local };
    bool _gizmo_edited{ false };
};
