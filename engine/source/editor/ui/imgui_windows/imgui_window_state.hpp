#pragma once

#include <memory>

#include "core/event.hpp"
#include "function/physics/physics_events.hpp"
#include "function/physics/physics_service.hpp"
#include "function/physics/physics_simulator.hpp"
#include "function/render/renderer_events.hpp"
#include "function/render/renderer_service.hpp"
#include "function/script/script_service.hpp"
#include "function/player/player_service.hpp"
#include "resource/scene_events.hpp"
#include "resource/scene_service.hpp"

#include "project/project.hpp"
#include "project/project_events.hpp"
#include "ui/imgui_window.hpp"
#include "ui/imgui.hpp"
#include "ui/ui_events.hpp"

namespace Arcadia
{
    struct ImguiWindowStateFunctor_Scene
    {
    public:
        void operator()(SceneService* scene_service);
    };

    struct ImguiWindowStateFunctor_Renderer
    {
    public:
        void operator()(RendererService* renderer_service);
    };

    struct ImguiWindowStateFunctor_PhysicsSimulator
    {
    public:
        void operator()(PhysicsService* physics_service);
    private:
        bool _EnabledModifyingTempAllocatorSize{ false };
        bool _ShouldLinkUpsAndSpu{ true };
    };

    struct ImguiWindowStateFunctor_ScriptInterpreter
    {
    public:
        void operator()(ScriptService* script_service);
    };

    struct ImguiWindowStateFunctor_PlayerController
    {
    public:
        void operator()(PlayerService* player_service);
    };

    struct ImguiWindowState: public ImguiWindowInterface
    {
    public:
        using SelfType = ImguiWindowState;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###state");

        ImguiWindowState(
            Engine& engine,
            bool open,
            std::string_view title
        );
        virtual ~ImguiWindowState() override = default;

        void OnEvent(EventBase& e);
        void OnUpdate();

    private:
        void _OnOpenImguiWindow(Events::OpenImguiWindow& e);

        void _DisplayState(
            std::string_view tab_name,
            bool is_activated,
            const std::function<void()>& display_fn
        ) const;

    private:
        ImguiWindowStateFunctor_Scene _ImguiWindowStateFunctor_Scene{};
        ImguiWindowStateFunctor_Renderer _ImguiWindowStateFunctor_Renderer{};
        ImguiWindowStateFunctor_PhysicsSimulator _ImguiWindowStateFunctor_PhysicsSimulator{};
        ImguiWindowStateFunctor_ScriptInterpreter _ImguiWindowStateFunctor_ScriptInterpreter{};
        ImguiWindowStateFunctor_PlayerController _ImguiWindowStateFunctor_PlayerController{};
    };
}