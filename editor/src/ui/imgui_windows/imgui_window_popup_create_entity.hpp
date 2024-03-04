#pragma once

#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"

#include"ui/imgui_windows/imgui_window.hpp"
#include"ui/ui_events.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_window_popup_create_entity: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_popup_create_entity;
    public:
        [[nodiscard]]
        static constexpr auto get_title() -> std::string
        {
            return "Create Entity";
        }

        using arcadia::imgui_window_interface::imgui_window_interface;
        virtual ~imgui_window_popup_create_entity() = default;


        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;

    private:
        void _on_new_entity(arcadia::event::new_entity& e);

    private:
        std::string _name{};
    };
}
