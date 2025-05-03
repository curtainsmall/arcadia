#pragma once

#include <memory>

#include "core/event.hpp"
#include "function/physics/physics_simulator.hpp"
#include "function/render/renderer.hpp"
#include "platform/api_def.hpp"
#include "ui/imgui_window.hpp"
#include "function/render/renderer_events.hpp"
#include "function/physics/physics_events.hpp"

#include "project/project.hpp"
#include "project/project_events.hpp"
#include "ui/ui_events.hpp"

namespace Arcadia
{
    class ImguiWindowStateFunctor_Scene
    {
    public:
        using SelfType = ImguiWindowStateFunctor_Scene;
    public:
        void operator()(const Scene& scene);
    };

    class ImguiWindowStateFunctor_Renderer
    {
    public:
        using SelfType = ImguiWindowStateFunctor_Renderer;
    public:
        void operator()(const RendererInterface& renderer);
    };

    class ImguiWindowStateFunctor_PhysicsSimulator
    {
    public:
        using SelfType = ImguiWindowStateFunctor_PhysicsSimulator;
    public:
        void operator()(PhysicsSimulator& physics_simulator);
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
        virtual ~ImguiWindowState() = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;

    private:
        void _OnOpenImguiWindow(Events::OpenImguiWindow& e);
        void _OnSceneActivated(Events::SceneActivated& e);
        void _OnSceneDeactivated(Events::SceneDeactivated& e);
        void _OnRendererBuilt(Events::RendererBuilt& e);
        void _OnRendererUnbuilt(Events::RendererUnbuilt& e);
        void _OnPhysicsSimulatorBuilt(Events::PhysicsSimulatorBuilt& e);
        void _OnPhysicsSimulatorUnbuilt(Events::PhysicsSimulatorUnbuilt& e);

    private:
        std::weak_ptr<Scene> _wpScene{};
        std::weak_ptr<RendererInterface> _wpRenderer{};
        std::weak_ptr<PhysicsSimulator> _wpPhysicsSimulator{};

        ImguiWindowStateFunctor_Scene _ImguiWindowStateFunctor_Scene{};
        ImguiWindowStateFunctor_Renderer _ImguiWindowStateFunctor_Renderer{};
        ImguiWindowStateFunctor_PhysicsSimulator _ImguiWindowStateFunctor_PhysicsSimulator{};
    };
}