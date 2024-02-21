#pragma once

#include<memory>

#include"core/app/app.hpp"
#include"function/window/window_layer.hpp"
#include"ui/imgui_layer.hpp"

namespace arcadia
{
    struct editor_app_layer: arcadia::app_layer
    {
    public:
        editor_app_layer();
        virtual ~editor_app_layer();

        virtual auto on_event(const arcadia::event& event) -> bool override;
        virtual void on_update(delta_time_type delta_time) override;


    private:
        auto _on_window_close(const arcadia::window_close& event) -> bool;

    private:
        arcadia::window_layer* _main_window_ptr{ nullptr };
        arcadia::imgui_layer* _main_ui_ptr{ nullptr };
    };

    ARCADIA_API auto create_application_uptr() -> std::unique_ptr<arcadia::app_layer>;
}
