#pragma once

#include <memory>

#include "core/event.hpp"
#include "function/physics/physics_events.hpp"
#include "function/physics/physics_layer.hpp"
#include "function/physics/physics_simulator.hpp"
#include "function/render/renderer_events.hpp"
#include "function/render/renderer_layer.hpp"
#include "platform/api_def.hpp"
#include "resource/scene_events.hpp"
#include "resource/scene_layer.hpp"

#include "project/project.hpp"
#include "project/project_events.hpp"
#include "ui/imgui_window.hpp"
#include "ui/ui_events.hpp"

namespace Arcadia
{
    class ImguiWindowStateFunctor_Scene
    {
    public:
        using SelfType = ImguiWindowStateFunctor_Scene;
    public:
        void operator()(const std::shared_ptr<SceneLayer>& scene_layer);
    };

    class ImguiWindowStateFunctor_Renderer
    {
    public:
        using SelfType = ImguiWindowStateFunctor_Renderer;
    public:
        void operator()(const std::shared_ptr<RendererLayer>& renderer_layer);
    };

    class ImguiWindowStateFunctor_PhysicsSimulator
    {
    public:
        using SelfType = ImguiWindowStateFunctor_PhysicsSimulator;
    public:
        void operator()(const std::shared_ptr<PhysicsLayer>& physics_layer);
    private:
        bool _EnabledModifyingTempAllocatorSize{ false };
        bool _ShouldLinkUpsAndSpu{ true };
    };

    class ImguiWindowState: public ImguiWindowInterface
    {
    public:
        using SelfType = ImguiWindowState;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###state");

        ImguiWindowState(
            bool open,
            const std::string& title
        );
        virtual ~ImguiWindowState() override = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;

    private:
        void _OnOpenImguiWindow(Events::OpenImguiWindow& e);

    private:
        ImguiWindowStateFunctor_Scene _ImguiWindowStateFunctor_Scene{};
        ImguiWindowStateFunctor_Renderer _ImguiWindowStateFunctor_Renderer{};
        ImguiWindowStateFunctor_PhysicsSimulator _ImguiWindowStateFunctor_PhysicsSimulator{};
    };
}