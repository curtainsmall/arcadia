#pragma once

#include<memory>

#include"core/app/app_layer.hpp"
#include"function/ui/imgui_layer.hpp"
#include"function/window/window_events.hpp"
#include"function/window/window_layer.hpp"

#include"project/project_events.hpp"
#include"project/project_layer.hpp"

namespace arcadia
{
    struct ARCADIA_API editor_app_layer: arcadia::app_layer
    {
    public:
        editor_app_layer();
        virtual ~editor_app_layer() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update(delta_time_type delta_time) override;
    private:
        void _imgui_window_installer(arcadia::imgui_layer& imgui_layer);
        void _stop();

        void _on_window_should_close(arcadia::event::window_should_close& e);
        void _on_project_unbuilt(arcadia::event::project_unbuilt& e);
        void _on_window_close_canceled(arcadia::event::window_close_canceled& e);

    private:
        bool _waiting_for_project_unbuilt_before_closing{ false };
    };

    ARCADIA_API auto create_application_uptr() -> std::unique_ptr<arcadia::app_layer>;
}
