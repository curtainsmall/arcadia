#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/input/input_events.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/render/renderer.hpp"
#include"resource/scene.hpp"
#include"ui/imgui_header.hpp"

#include"project/project.hpp"
#include"project/project_events.hpp"
#include"ui/imgui_window.hpp"
#include"ui/ui_events.hpp"

class ImguiWindowViewport: public iImguiWindow
{
public:
    enum class GizmoOption
    {
        None = 0,
        Translation = ImGuizmo::OPERATION::TRANSLATE,
        Rotation = ImGuizmo::OPERATION::ROTATE,
        Scale = ImGuizmo::OPERATION::SCALE,
    };
    enum class GizmoMode
    {
        Local = ImGuizmo::MODE::LOCAL,
        World = ImGuizmo::MODE::WORLD,
    };
    using SelfType = ImguiWindowViewport;
public:
    ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###viewport");

    inline ImguiWindowViewport(
        bool open,
        const std::string& title
    ) :
        iImguiWindow(open, title)
    {}
    virtual ~ImguiWindowViewport() = default;

    virtual void OnEvent(EventBase& e) override;
    virtual void OnUpdate() override;

private:
    void _OnInputCursorMove(Events::InputCursorMove& e);
    void _OnOpenImguiWindow(Events::OpenImguiWindow& e);
    void _OnProjectBuilt(Events::ProjectBuilt& e);
    void _OnProjectUnbuilt(Events::ProjectUnbuilt& e);
    void _OnSceneActivated(Events::SceneActivated& e);
    void _OnSceneDeactivated(Events::SceneDeactivated& e);
    void _OnSelectEntity(Events::SelectEntity& e);
    void _OnRenameEntity(Events::RenameEntity& e);
    void _OnDeleteEntity(Events::DeleteEntity& e);
    void _OnRendererBuilt(Events::RendererBuilt& e);
    void _OnRendererUnbuilt(Events::RendererUnbuilt& e);
    void _OnPhysicsSimulatorBuilt(Events::PhysicsSimulatorBuilt& e);
    void _OnPhysicsSimulatorUnbuilt(Events::PhysicsSimulatorUnbuilt& e);
    void _OnShowGizmo(Events::ShowGizmo& e);

public:
    static inline std::string ViewportCameraEntityName{ "viewport_camera" };
private:
    std::weak_ptr<Project> _Project{};
    std::weak_ptr<Scene> _Scene{};
    std::weak_ptr<iRenderer> _Renderer{};
    std::weak_ptr<PhysicsSimulator> _PhysicsSimulator{};

    std::string _SelectedEntityName{};

    glm::vec2 _CursorMoveDistance{};

    bool _InViewportFreecamMode{ false };
    bool _ShowGizmo{ false };

    GizmoOption _GizmoOption{ GizmoOption::None };
    GizmoMode _GizmoMode{ GizmoMode::Local };
    bool _GizmoEdited{ false };
};