#pragma once

#include<functional>
#include<memory>
#include<vector>

#include"core/exception.hpp"
#include"function/ui/imgui_header.hpp"
#include"function/ui/imgui_style.hpp"
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
        imgui_layer(
            const std::shared_ptr<const arcadia::window_layer>& window_layer_sptr,
            const std::function<void(arcadia::imgui_layer&)>& imgui_window_installer ={},
            const std::function<void()>& imgui_style_setter = arcadia::imgui_style_dark
        );
        virtual ~imgui_layer();

        [[nodiscard]]
        inline auto get_window_sptr() const -> std::shared_ptr<const arcadia::window_layer>
        {
            return _window_wptr.lock();
        }

        [[nodiscard]]
        inline auto get_imgui_window_uptrs() const -> const std::vector<std::unique_ptr<arcadia::imgui_window_interface>>&
        {
            return _imgui_window_uptrs;
        }

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;

        template<arcadia::imgui_window_like ImGuiWindow, class ...Args>
        auto emplace_imgui_window(Args&& ...args) -> self_type&
        {
            _imgui_window_uptrs.emplace_back(std::make_unique<ImGuiWindow>(std::forward<Args>(args)...));
            return *this;
        }
    public:
        bool show_demo_window{ false };
        bool show_debug_info{ false };
    private:
        std::weak_ptr<const arcadia::window_layer> _window_wptr;
        ImGuiContext* _imgui_context_ptr{ nullptr };
        std::vector<std::unique_ptr<arcadia::imgui_window_interface>> _imgui_window_uptrs{};
    };
}
