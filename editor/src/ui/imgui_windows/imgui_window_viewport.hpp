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

namespace Arcadia
{
    struct ARCADIA_API ImguiWindowViewport: Arcadia::iImguiWindow
    {
    public:
        enum struct GizmoOption: int
        {
            None = 0,
            Translation = ImGuizmo::OPERATION::TRANSLATE,
            Rotation = ImGuizmo::OPERATION::ROTATE,
            Scale = ImGuizmo::OPERATION::SCALE,
        };
        enum struct GizmoMode: int
        {
            Local = ImGuizmo::MODE::LOCAL,
            World = ImGuizmo::MODE::WORLD,
        };
        using self_type = ImguiWindowViewport;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS("###viewport");

        inline ImguiWindowViewport(
            bool Open,
            const std::string& title
        ):
            Arcadia::iImguiWindow(Open, title)
        {}
        virtual ~ImguiWindowViewport() = default;

        virtual void OnEvent(Arcadia::EventBase& event) override;
        virtual void OnUpdate() override;

    private:
        void _OnInputCursorMove(Arcadia::Event::InputCursorMove& e);
        void _OnOpenImguiWindow(Arcadia::Event::OpenImguiWindow& e);
        void _OnProjectBuilt(Arcadia::Event::ProjectBuilt& e);
        void _OnProjectUnbuilt(Arcadia::Event::ProjectUnbuilt& e);
        void _OnSceneActivated(Arcadia::Event::SceneActivated& e);
        void _OnSceneDeactivated(Arcadia::Event::SceneDeactivated& e);
        void _OnSelectEntity(Arcadia::Event::SelectEntity& e);
        void _OnRenameEntity(Arcadia::Event::RenameEntity& e);
        void _OnDeleteEntity(Arcadia::Event::DeleteEntity& e);
        void _OnRendererBuilt(Arcadia::Event::RendererBuilt& e);
        void _OnRendererUnbuilt(Arcadia::Event::RendererUnbuilt& e);
        void _OnPhysicsSimualtorBuilt(Arcadia::Event::PhysicsSimulatorBuilt& e);
        void _OnPhysicsSimulatorUnbuilt(Arcadia::Event::PhysicsSimulatorUnbuilt& e);

    public:
        static inline std::string ViewportCameraEntityName{ "viewport_camera" };
    private:
        std::weak_ptr<Arcadia::Project> _Project{};
        std::weak_ptr<Arcadia::Scene> _Scene{};
        std::weak_ptr<Arcadia::iRenderer> _Renderer{};
        std::weak_ptr<Arcadia::PhysicsSimulator> _PhysicsSimulator{};

        std::string _SelectedEntityName{};

        glm::vec2 _CursorMove{};

        bool _InViewportFreeCam{ false };

        GizmoOption _GizmoOption{ GizmoOption::None };
        GizmoMode _GizmoMode{ GizmoMode::Local };
        bool _GizmoEdited{ false };
    };
}
