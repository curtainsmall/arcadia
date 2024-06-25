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
    void _on_input_cursor_move(events::InputCursorMove& e);
    void _on_open_imgui_window(events::OpenImguiWindow& e);
    void _on_project_built(events::ProjectBuilt& e);
    void _on_project_unbuilt(events::ProjectUnbuilt& e);
    void _on_scene_activated(events::SceneActivated& e);
    void _on_scene_deactivated(events::SceneDeactivated& e);
    void _on_select_entity(events::SelectEntity& e);
    void _on_rename_entity(events::RenameEntity& e);
    void _on_delete_entity(events::DeleteEntity& e);
    void _on_renderer_built(events::RendererBuilt& e);
    void _on_renderer_unbuilt(events::RendererUnbuilt& e);
    void _on_physics_simulator_built(events::PhysicsSimulatorBuilt& e);
    void _on_physics_simulator_unbuilt(events::PhysicsSimulatorUnbuilt& e);
    void _on_show_gizmo(events::ShowGizmo& e);

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
