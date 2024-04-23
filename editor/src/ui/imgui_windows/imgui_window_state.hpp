#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/render/renderer.hpp"
#include"ui/imgui_window.hpp"

#include"project/project.hpp"
#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace Arcadia
{
    struct ARCADIA_API ImguiWindowStateScene
    {
    public:
        using self_type = ImguiWindowStateScene;
    public:
        void operator()(const Arcadia::Scene& scene);
    };

    struct ARCADIA_API ImguiWindowStateRenderer
    {
    public:
        using self_type = ImguiWindowStateRenderer;
    public:
        void operator()(const Arcadia::iRenderer& renderer);
    };

    struct ARCADIA_API ImguiWindowStatePhysicsSimulator
    {
    public:
        using self_type = ImguiWindowStatePhysicsSimulator;
    public:
        void operator()(Arcadia::PhysicsSimulator& physics_simulator);
    private:
        bool _EnableModifyingTempAllocatorSize{ false };
        bool _LinkUpsAndSpu{ true };
    };

    struct ARCADIA_API ImguiWindowState: Arcadia::iImguiWindow
    {
    public:
        using self_type = ImguiWindowState;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS("###state");

        inline ImguiWindowState(
            bool open,
            const std::string& title
        ):
            Arcadia::iImguiWindow(open, title)
        {}
        virtual ~ImguiWindowState() = default;

        virtual void OnEvent(Arcadia::EventBase& event) override;
        virtual void OnUpdate() override;

    private:
        void _OnOpenImguiWindow(Arcadia::Event::OpenImguiWindow& e);
        void _OnSceneActivated(Arcadia::Event::SceneActivated& e);
        void _OnSceneDeactivated(Arcadia::Event::SceneDeactivated& e);
        void _OnRendererBuilt(Arcadia::Event::RendererBuilt& e);
        void _OnRendererUnbuilt(Arcadia::Event::RendererUnbuilt& e);
        void _OnPhysicsSimualtorBuilt(Arcadia::Event::PhysicsSimulatorBuilt& e);
        void _OnPhysicsSimulatorUnbuilt(Arcadia::Event::PhysicsSimulatorUnbuilt& e);

    private:
        std::weak_ptr<Arcadia::Scene> _Scene{};
        std::weak_ptr<Arcadia::iRenderer> _Renderer{};
        std::weak_ptr<Arcadia::PhysicsSimulator> _PhysicsSimulator{};

        Arcadia::ImguiWindowStateScene _ImguiWindowStateScene{};
        Arcadia::ImguiWindowStateRenderer _ImguiWindowStateRenderer{};
        Arcadia::ImguiWindowStatePhysicsSimulator _ImguiWindowStatePhysicsSimulator{};
    };
}
