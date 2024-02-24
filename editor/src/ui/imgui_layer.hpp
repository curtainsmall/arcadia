#pragma once

#include"core/exception.hpp"
#include"function/window/window_layer.hpp"
#include"ui/imgui_header.hpp"

namespace arcadia
{
    ARCADIA_EXCEPTION(imgui_error);

    struct ARCADIA_API imgui_layer: arcadia::layer_interface
    {
    public:
        imgui_layer(arcadia::window_layer& window);
        virtual ~imgui_layer();

        virtual auto on_event(const arcadia::event& event) -> bool override;
        virtual void on_update(delta_time_type delta_time) override;

    public:
        bool show_demo_window{ true };
        bool show_debug_info{ true };
    private:
        arcadia::window_layer* _window_ptr;
        ImGuiContext* _imgui_context_ptr{ nullptr };


    };
}
