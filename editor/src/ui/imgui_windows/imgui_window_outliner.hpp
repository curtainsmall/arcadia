#pragma once

#include<string>

#include"core/base.hpp"
#include"ui/imgui_windows/imgui_window.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_outliner: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_outliner;
    public:
        using arcadia::imgui_window_interface::imgui_window_interface;
        virtual ~imgui_window_outliner() = default;

        virtual void on_event(arcadia::event_base& event);
        virtual void on_update();

        [[nodiscard]]
        virtual inline auto get_title() const -> std::string override
        {
            return "Outliner";
        }
    };
}
