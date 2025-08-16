#pragma once

#include <memory>

#include "core/event.hpp"
#include "function/physics/physics_events.hpp"
#include "function/physics/physics_layer.hpp"
#include "function/physics/physics_simulator.hpp"
#include "function/render/renderer_events.hpp"
#include "function/render/renderer_layer.hpp"
#include "function/script/script_layer.hpp"
#include "function/player/player_layer.hpp"
#include "resource/scene_events.hpp"
#include "resource/scene_layer.hpp"

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
        void operator()(const std::shared_ptr<SceneLayer>& scene_layer);
    };

    struct ImguiWindowStateFunctor_Renderer
    {
    public:
        void operator()(const std::shared_ptr<RendererLayer>& renderer_layer);
    };

    struct ImguiWindowStateFunctor_PhysicsSimulator
    {
    public:
        void operator()(const std::shared_ptr<PhysicsLayer>& physics_layer);
    private:
        bool _EnabledModifyingTempAllocatorSize{ false };
        bool _ShouldLinkUpsAndSpu{ true };
    };

    struct ImguiWindowStateFunctor_ScriptInterpreter
    {
    public:
        void operator()(const std::shared_ptr<ScriptLayer>& script_layer);
    };

    struct ImguiWindowStateFunctor_PlayerController
    {
    public:
        void operator()(const std::shared_ptr<PlayerLayer>& player_layer);
    };

    struct ImguiWindowState: public ImguiWindowInterface
    {
    public:
        using SelfType = ImguiWindowState;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###state");

        ImguiWindowState(
            bool open,
            std::string_view title
        );
        virtual ~ImguiWindowState() override = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;

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