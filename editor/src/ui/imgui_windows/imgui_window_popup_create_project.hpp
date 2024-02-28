#pragma once

#include<string>

#include"core/base.hpp"

#include"ui/imgui_windows/imgui_window.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_popup_create_project: arcadia::imgui_window_interface
    {
    public:
        using arcadia::imgui_window_interface::imgui_window_interface;
        virtual ~imgui_window_popup_create_project() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;
    private:
        std::string _name{};
        std::string _filepath_str{};
        bool _display_empty_name_waring{ false };
    };
}
