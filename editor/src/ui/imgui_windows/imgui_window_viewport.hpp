#pragma once

#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
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
        ARCADIA_IMGUI_WINDOW_ID_STR("###viewport");

        using arcadia::imgui_window_interface::imgui_window_interface;
        virtual ~imgui_window_viewport() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;

    private:
        void _on_open_window(arcadia::event::open_imgui_window& e);
        void _on_scene_activated(arcadia::event::scene_activated& e);
        void _on_scene_deactivated(arcadia::event::scene_deactivated& e);

    private:
        const arcadia::scene* _scene_cptr{};
        arcadia::renderer_interface* _renderer_ptr{};
    };
}
