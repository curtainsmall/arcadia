#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"resource/scene/scene.hpp"

#include"function/ui/imgui_window.hpp"
#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_popup_create_entity: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_popup_create_entity;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR("###popup_create_entity");

        using arcadia::imgui_window_interface::imgui_window_interface;
        virtual ~imgui_window_popup_create_entity() = default;


        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;

    private:
        void _on_new_entity(arcadia::event::new_entity& e);
        void _on_scene_activated(arcadia::event::scene_activated& e);
        void _on_scene_deactivated(arcadia::event::scene_deactivated& e);

    private:
        std::weak_ptr<const arcadia::scene> _scene_wptr{};
        std::string _name{};
        bool _name_available{ true };
    };
}
