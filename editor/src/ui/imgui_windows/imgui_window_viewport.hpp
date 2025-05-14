#pragma once

#include <memory>
#include <string>

#include "core/event.hpp"
#include "function/input/input_events.hpp"
#include "function/physics/physics_events.hpp"
#include "function/physics/physics_simulator.hpp"
#include "function/render/renderer_events.hpp"
#include "platform/api_def.hpp"
#include "resource/scene_events.hpp"

#include "project/project.hpp"
#include "project/project_events.hpp"
#include "ui/imgui.hpp"
#include "ui/imgui_window.hpp"
#include "ui/ui_events.hpp"

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
        enum class GizmoEditState
        {
            None,
            Editing,
            Edited,
        };
        using SelfType = ImguiWindowViewport;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###viewport");

        ImguiWindowViewport(
            bool open,
            const std::string& title
        );
        virtual ~ImguiWindowViewport() override = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;

    private:
        void _OnInputCursorMove(Events::InputCursorMove& e);
        void _OnOpenImguiWindow(Events::OpenImguiWindow& e);
        void _OnSceneActivated(Events::SceneActivated& e);
        void _OnSceneDeactivated(Events::SceneDeactivated& e);
        void _OnSelectEntity(Events::SelectEntity& e);
        void _OnDeleteEntity(Events::DeleteEntity& e);
        void _OnShowGizmo(Events::ShowGizmo& e);
        void _OnKeyboardInputOccupied(Events::KeyboardInputOccupied& e);

    private:
        static inline std::string _ViewportCameraEntityName{ "viewport_camera" };

        EntityId _SelectedEntityId{};
        EntityId _ViewportCameraEntityId{};

        glm::vec2 _CursorMoveDistance{};

        bool _InViewportFreecamMode{ false };

        bool _ShowGizmo{ true };
        GizmoOption _GizmoOption{ GizmoOption::None };
        GizmoMode _GizmoMode{ GizmoMode::Local };
        GizmoEditState _GizmoEditState{ GizmoEditState::None };
        bool _GizmoShortcutAvailable{ true };

        glm::vec3 _GizmoOriginPosition{};
        glm::vec3 _GizmoOriginRotationEularAngle{};
        glm::vec3 _GizmoOriginScale{};
    };
}