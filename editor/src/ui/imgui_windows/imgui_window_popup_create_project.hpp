#pragma once

#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"

#include"ui/imgui_windows/imgui_window.hpp"
#include"ui/ui_events.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_popup_create_project: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_popup_create_project;
    public:
        [[nodiscard]]
        static inline auto get_title() -> std::string
        {
            return "Create Project";
        }

        using arcadia::imgui_window_interface::imgui_window_interface;
        virtual ~imgui_window_popup_create_project() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;


    private:
        void _on_new_project(arcadia::event::new_project& e);
    private:
        std::string _name{};
        std::string _filepath_str{};
        bool _display_empty_name_warning{ false };
    };
}
