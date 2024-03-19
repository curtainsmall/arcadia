#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/ui/imgui_window.hpp"

#include"project/project.hpp"
#include"project/project_events.hpp"
#include"ui/ui_events.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_popup_create_scene: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_popup_create_scene;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS("###popup_create_scene");

        using arcadia::imgui_window_interface::imgui_window_interface;
        virtual ~imgui_window_popup_create_scene() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;

    private:
        void _on_project_built(arcadia::event::project_built& e);
        void _on_project_unbuilt(arcadia::event::project_unbuilt& e);
        void _on_new_scene(arcadia::event::new_scene& e);


    private:
        std::weak_ptr<const arcadia::project> _project_wptr{};
        std::string _name{};
        bool _as_current{ true };
        bool _name_available{ true };
    };
}
