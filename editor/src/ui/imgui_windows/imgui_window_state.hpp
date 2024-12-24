#pragma once

#include<memory>

#include"core/event/event.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/render/renderer.hpp"
#include"platform/api_def.hpp"
#include"ui/imgui_window.hpp"

#include"project/project.hpp"
#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace Arcadia
{
    class ImguiWindowStateScene
    {
    public:
        using SelfType = ImguiWindowStateScene;
    public:
        void operator()(const Scene& scene);
    };

    class ImguiWindowStateRenderer
    {
    public:
        using SelfType = ImguiWindowStateRenderer;
    public:
        void operator()(const RendererInterface& renderer);
    };

    class ImguiWindowStatePhysicsSimulator
    {
    public:
        using SelfType = ImguiWindowStatePhysicsSimulator;
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

        inline ImguiWindowState(
            bool open,
            const std::string& title
        ) :
            ImguiWindowInterface(open, title)
        {}
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
        std::weak_ptr<Scene> _SceneWeakPtr{};
        std::weak_ptr<RendererInterface> _Renderer{};
        std::weak_ptr<PhysicsSimulator> _PhysicsSimulator{};

        ImguiWindowStateScene _ImguiWindowStateScene{};
        ImguiWindowStateRenderer _ImguiWindowStateRenderer{};
        ImguiWindowStatePhysicsSimulator _ImguiWindowStatePhysicsSimulator{};
    };
}