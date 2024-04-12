#pragma once

#include<memory>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/render/renderer.hpp"
#include"function/ui/imgui_window.hpp"

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
        void operator()(const std::shared_ptr<Arcadia::Scene>& sp_scene);
    };

    struct ARCADIA_API ImguiWindowStateRenderer
    {
    public:
        using self_type = ImguiWindowStateRenderer;
    public:
        void operator()(const std::shared_ptr<Arcadia::iRenderer>& sp_renderer);
    };

    struct ARCADIA_API ImguiWindowStatePhysicsSimulator
    {
    public:
        using self_type = ImguiWindowStatePhysicsSimulator;
    public:
        void operator()(const std::shared_ptr<Arcadia::PhysicsSimulator>& sp_physics_simulator);
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
        std::shared_ptr<Arcadia::Scene> _spScene{};
        std::shared_ptr<Arcadia::iRenderer> _spRenderer{};
        std::shared_ptr<Arcadia::PhysicsSimulator> _spPhysicsSimulator{};

        Arcadia::ImguiWindowStateScene _ImguiWindowStateScene{};
        Arcadia::ImguiWindowStateRenderer _ImguiWindowStateRenderer{};
        Arcadia::ImguiWindowStatePhysicsSimulator _ImguiWindowStatePhysicsSimulator{};
    };
}
