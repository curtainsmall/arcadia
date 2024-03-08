#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/input/input_events.hpp"
#include"function/render/renderer.hpp"
#include"resource/scene/scene.hpp"

#include"function/ui/imgui_window.hpp"
#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_viewport: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_viewport;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR_GETERS("###viewport");

        using arcadia::imgui_window_interface::imgui_window_interface;
        virtual ~imgui_window_viewport() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;

    private:
        void _on_input_cursor_move(arcadia::event::input_cursor_move& e);
        void _on_open_imgui_window(arcadia::event::open_imgui_window& e);
        void _on_scene_activated(arcadia::event::scene_activated& e);
        void _on_scene_deactivated(arcadia::event::scene_deactivated& e);
        void _on_renderer_built(arcadia::event::renderer_built& e);
        void _on_renderer_unbuilt(arcadia::event::renderer_unbuilt& e);
    private:
        std::weak_ptr<arcadia::scene> _scene_wptr{};
        std::weak_ptr<arcadia::renderer_interface> _renderer_wptr{};

        arcadia::camera_component _camera{};
        arcadia::model_component _grid{};

        glm::vec2 _cursor_move{};
    };
}
