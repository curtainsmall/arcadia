#pragma once

#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"ui/imgui_windows/imgui_window.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_viewport: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_viewport;
    public:
        using arcadia::imgui_window_interface::imgui_window_interface;
        virtual ~imgui_window_viewport() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;

        [[nodiscard]]
        inline auto get_title() const->std::string override
        {
            return "Viewport";
        }
    };
}
