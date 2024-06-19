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
    bool _enable_modifying_temp_allocator_size{ false };
    bool _link_ups_and_spu{ true };
};

struct ImguiWindowState: iImguiWindow
{
public:
    using self_type = ImguiWindowState;
public:
    ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###state");

    inline ImguiWindowState(
        bool open,
        const std::string& title
    ):
        iImguiWindow(open, title)
    {}
    virtual ~ImguiWindowState() = default;

    virtual void on_event(EventBase& e) override;
    virtual void on_update() override;

private:
    void _on_open_imgui_window(event::OpenImguiWindow& e);
    void _on_scene_activated(event::SceneActivated& e);
    void _on_scene_deactivated(event::SceneDeactivated& e);
    void _on_renderer_built(event::RendererBuilt& e);
    void _on_renderer_unbuilt(event::RendererUnbuilt& e);
    void _on_physics_simulator_built(event::PhysicsSimulatorBuilt& e);
    void _on_physics_simulator_unbuilt(event::PhysicsSimulatorUnbuilt& e);

private:
    std::weak_ptr<Scene> _scene{};
    std::weak_ptr<iRenderer> _renderer{};
    std::weak_ptr<PhysicsSimulator> _physics_simulator{};

    ImguiWindowStateScene _imgui_window_state_scene{};
    ImguiWindowStateRenderer _imgui_window_state_renderer{};
    ImguiWindowStatePhysicsSimulator _imgui_window_state_physics_simulator{};
};
