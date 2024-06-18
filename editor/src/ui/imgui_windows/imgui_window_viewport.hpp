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
        bool Open,
        const std::string& title
    ):
        iImguiWindow(Open, title)
    {}
    virtual ~ImguiWindowViewport() = default;

    virtual void OnEvent(EventBase& event) override;
    virtual void OnUpdate() override;

private:
    void _OnInputCursorMove(Event::InputCursorMove& e);
    void _OnOpenImguiWindow(Event::OpenImguiWindow& e);
    void _OnProjectBuilt(Event::ProjectBuilt& e);
    void _OnProjectUnbuilt(Event::ProjectUnbuilt& e);
    void _OnSceneActivated(Event::SceneActivated& e);
    void _OnSceneDeactivated(Event::SceneDeactivated& e);
    void _OnSelectEntity(Event::SelectEntity& e);
    void _OnRenameEntity(Event::RenameEntity& e);
    void _OnDeleteEntity(Event::DeleteEntity& e);
    void _OnRendererBuilt(Event::RendererBuilt& e);
    void _OnRendererUnbuilt(Event::RendererUnbuilt& e);
    void _OnPhysicsSimualtorBuilt(Event::PhysicsSimulatorBuilt& e);
    void _OnPhysicsSimulatorUnbuilt(Event::PhysicsSimulatorUnbuilt& e);
    void _OnShowGizmo(Event::ShowGizmo& e);

public:
    static inline std::string ViewportCameraEntityName{ "viewport_camera" };
private:
    std::weak_ptr<Project> _Project{};
    std::weak_ptr<Scene> _Scene{};
    std::weak_ptr<iRenderer> _Renderer{};
    std::weak_ptr<PhysicsSimulator> _PhysicsSimulator{};

    std::string _SelectedEntityName{};

    glm::vec2 _CursorMove{};

    bool _InViewportFreeCam{ false };
    bool _ShowGizmo{ false };

    GizmoOption _GizmoOption{ GizmoOption::None };
    GizmoMode _GizmoMode{ GizmoMode::Local };
    bool _GizmoEdited{ false };
};
