#pragma once

#include<functional>
#include<vector>

#include"core/exception.hpp"
#include"function/ui/imgui_header.hpp"
#include"function/ui/imgui_window.hpp"
#include"function/window/window_layer.hpp"


namespace arcadia
{
    ARCADIA_EXCEPTION(imgui_error);

    struct ARCADIA_API imgui_layer: arcadia::layer_interface
    {
    public:
        using self_type = imgui_layer;
    public:
        imgui_layer(arcadia::window_layer& window, const std::function<void(arcadia::imgui_layer&)>& imgui_window_installer ={});
        virtual ~imgui_layer();

        [[nodiscard]]
        inline auto get_window() const -> const arcadia::window_layer&
        {
            return *_window_ptr;
        }

        [[nodiscard]]
        inline auto get_imgui_window_uptrs() const -> const std::vector<std::unique_ptr<arcadia::imgui_window_interface>>&
        {
            return _imgui_window_uptrs;
        }

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update(delta_time_type delta_time) override;

        template<arcadia::imgui_window_like ImGuiWindow, class ...Args>
        auto emplace_imgui_window(Args&& ...args) -> self_type&
        {
            _imgui_window_uptrs.emplace_back(std::make_unique<ImGuiWindow>(std::forward<Args>(args)...));
            return *this;
        }
    public:
        bool show_demo_window{ true };
        bool show_debug_info{ false };
    private:
        arcadia::window_layer* _window_ptr;
        ImGuiContext* _imgui_context_ptr{ nullptr };
        std::vector<std::unique_ptr<arcadia::imgui_window_interface>> _imgui_window_uptrs{};
    };
}
