#pragma once

#include<vector>

#include"core/exception.hpp"
#include"function/window/window_layer.hpp"

#include"ui/imgui_header.hpp"
#include"ui/imgui_windows/imgui_window.hpp"

namespace arcadia
{
    ARCADIA_EXCEPTION(imgui_error);

    struct ARCADIA_API imgui_layer: arcadia::layer_interface
    {
    public:
        using self_type = imgui_layer;
    public:
        imgui_layer(arcadia::window_layer& window);
        virtual ~imgui_layer();

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update(delta_time_type delta_time) override;

    private:
        template<arcadia::imgui_window_like ImGuiWindow, class ...Args>
        void _emplace_imgui_window(Args&& ...args)
        {
            _imgui_window_uptrs.emplace_back(std::make_unique<ImGuiWindow>(std::forward<Args>(args)...));
        }
    public:
        bool show_demo_window{ false };
        bool show_debug_info{ false };
    private:
        arcadia::window_layer* _window_ptr;
        ImGuiContext* _imgui_context_ptr{ nullptr };
        std::vector<std::unique_ptr<arcadia::imgui_window_interface>> _imgui_window_uptrs{};
    };
}
