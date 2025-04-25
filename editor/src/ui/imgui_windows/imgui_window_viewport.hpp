#pragma once

#include<memory>
#include<string>

#include"core/event/event.hpp"
#include"function/input/input_events.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/render/renderer.hpp"
#include"platform/api_def.hpp"
#include"resource/scene.hpp"
#include"ui/imgui_header.hpp"

#include"project/project.hpp"
#include"project/project_events.hpp"
#include"ui/imgui_window.hpp"
#include"ui/ui_events.hpp"

namespace Arcadia
{
    class ImguiWindowViewport: public ImguiWindowInterface
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
        ):
            ImguiWindowInterface(open, title)
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
        void _OnDeleteEntity(Events::DeleteEntity& e);
        void _OnRendererBuilt(Events::RendererBuilt& e);
        void _OnRendererUnbuilt(Events::RendererUnbuilt& e);
        void _OnPhysicsSimulatorBuilt(Events::PhysicsSimulatorBuilt& e);
        void _OnPhysicsSimulatorUnbuilt(Events::PhysicsSimulatorUnbuilt& e);
        void _OnShowGizmo(Events::ShowGizmo& e);

    private:
        static inline std::string _ViewportCameraEntityName{ "viewport_camera" };

        std::weak_ptr<Project> _wpProject{};
        std::weak_ptr<Scene> _wpScene{};
        std::weak_ptr<RendererInterface> _wpRenderer{};
        std::weak_ptr<PhysicsSimulator> _wpPhysicsSimulator{};

        EntityId _SelectedEntityId{};

        glm::vec2 _CursorMoveDistance{};

        bool _InViewportFreecamMode{ false };
        bool _ShowGizmo{ true };

        GizmoOption _GizmoOption{ GizmoOption::None };
        GizmoMode _GizmoMode{ GizmoMode::Local };
        bool _GizmoEdited{ false };
    };
}