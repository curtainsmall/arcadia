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

struct ImguiWindowStateScene
{
public:
    using self_type = ImguiWindowStateScene;
public:
    void operator()(const Scene& scene);
};

struct ImguiWindowStateRenderer
{
public:
    using self_type = ImguiWindowStateRenderer;
public:
    void operator()(const iRenderer& renderer);
};

struct ImguiWindowStatePhysicsSimulator
{
public:
    using self_type = ImguiWindowStatePhysicsSimulator;
public:
    void operator()(PhysicsSimulator& physics_simulator);
private:
    bool _EnableModifyingTempAllocatorSize{ false };
    bool _LinkUpsAndSpu{ true };
};

struct ImguiWindowState: iImguiWindow
{
public:
    using self_type = ImguiWindowState;
public:
    ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS("###state");

    inline ImguiWindowState(
        bool open,
        const std::string& title
    ):
        iImguiWindow(open, title)
    {}
    virtual ~ImguiWindowState() = default;

    virtual void OnEvent(EventBase& event) override;
    virtual void OnUpdate() override;

private:
    void _OnOpenImguiWindow(Event::OpenImguiWindow& e);
    void _OnSceneActivated(Event::SceneActivated& e);
    void _OnSceneDeactivated(Event::SceneDeactivated& e);
    void _OnRendererBuilt(Event::RendererBuilt& e);
    void _OnRendererUnbuilt(Event::RendererUnbuilt& e);
    void _OnPhysicsSimualtorBuilt(Event::PhysicsSimulatorBuilt& e);
    void _OnPhysicsSimulatorUnbuilt(Event::PhysicsSimulatorUnbuilt& e);

private:
    std::weak_ptr<Scene> _Scene{};
    std::weak_ptr<iRenderer> _Renderer{};
    std::weak_ptr<PhysicsSimulator> _PhysicsSimulator{};

    ImguiWindowStateScene _ImguiWindowStateScene{};
    ImguiWindowStateRenderer _ImguiWindowStateRenderer{};
    ImguiWindowStatePhysicsSimulator _ImguiWindowStatePhysicsSimulator{};
};
