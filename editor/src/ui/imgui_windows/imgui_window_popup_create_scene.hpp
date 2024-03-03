#pragma once

#include<string>

#include"core/base.hpp"

#include"project/project.hpp"
#include"ui/imgui_windows/imgui_window.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_popup_create_scene: arcadia::imgui_window_interface
    {
    public:
        using arcadia::imgui_window_interface::imgui_window_interface;
        virtual ~imgui_window_popup_create_scene() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;

        [[nodiscard]]
        virtual inline auto get_title() const -> std::string override
        {
            return "Create Scene";
        }

    private:
        const arcadia::project* _project_ptr{};
        std::string _name{};
        bool _as_current{ true };
        bool _name_available{ true };
        bool _display_empty_name_waring{ false };
    };
}
