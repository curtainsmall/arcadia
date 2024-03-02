#pragma once

#include<memory>

#include"core/app/app_layer.hpp"
#include"function/window/window_events.hpp"
#include"function/window/window_layer.hpp"

#include"project/project_layer.hpp"
#include"ui/imgui_layer.hpp"

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
        void _on_window_close(const arcadia::event::window_close& e);
    private:
        arcadia::window_layer* _main_window_ptr{};
        arcadia::project_layer* _project_ptr{};
        arcadia::imgui_layer* _main_ui_ptr{};
    };

    ARCADIA_API auto create_application_uptr() -> std::unique_ptr<arcadia::app_layer>;
}
