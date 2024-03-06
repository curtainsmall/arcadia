#pragma once

#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"

#include"function/ui/imgui_window.hpp"
#include"ui/ui_events.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_popup_create_project: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_popup_create_project;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR("###popup_create_project");

        using arcadia::imgui_window_interface::imgui_window_interface;
        virtual ~imgui_window_popup_create_project() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;


    private:
        void _on_new_project(arcadia::event::new_project& e);
    private:
        std::string _name{};
        std::string _filepath_str{};
        bool _display_empty_name_warning{ true };
    };
}
