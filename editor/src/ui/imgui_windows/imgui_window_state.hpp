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

namespace arcadia
{
    struct ARCADIA_API imgui_window_state_scene
    {
    public:
        using self_type = imgui_window_state_scene;
    public:
        void operator()(const std::shared_ptr<arcadia::scene>& scene_sptr);
    };

    struct ARCADIA_API imgui_window_state_renderer
    {
    public:
        using self_type = imgui_window_state_renderer;
    public:
        void operator()(const std::shared_ptr<arcadia::renderer_interface>& renderer_sptr);
    };

    struct ARCADIA_API imgui_window_state_physics_simulator
    {
    public:
        using self_type = imgui_window_state_physics_simulator;
    public:
        void operator()(const std::shared_ptr<arcadia::physics_simulator>& physics_simulator_sptr);
    private:
        bool _enable_modifying_temp_allocator_size{ false };
        bool _link_ups_and_spu{ true };
    };

    struct ARCADIA_API imgui_window_state:arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_state;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS("###state");

        inline imgui_window_state(
            bool open,
            const std::string& title
        ):
            imgui_window_interface(open, title)
        {}
        virtual ~imgui_window_state() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;

    private:
        void _on_open_imgui_window(arcadia::event::open_imgui_window& e);
        void _on_scene_activated(arcadia::event::scene_activated& e);
        void _on_scene_deactivated(arcadia::event::scene_deactivated& e);
        void _on_renderer_built(arcadia::event::renderer_built& e);
        void _on_renderer_unbuilt(arcadia::event::renderer_unbuilt& e);
        void _on_physics_simulator_built(arcadia::event::physics_simulator_built& e);
        void _on_physics_simulator_unbuilt(arcadia::event::physics_simulator_unbuilt& e);

    private:
        std::weak_ptr<arcadia::scene> _scene_wptr{};
        std::weak_ptr<arcadia::renderer_interface> _renderer_wptr{};
        std::weak_ptr<arcadia::physics_simulator> _physics_simulator_wptr{};

        arcadia::imgui_window_state_scene _imgui_window_state_scene{};
        arcadia::imgui_window_state_renderer _imgui_window_state_renderer{};
        arcadia::imgui_window_state_physics_simulator _imgui_window_state_physics_simulator{};
    };
}
