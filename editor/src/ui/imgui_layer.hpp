#pragma once

#include<functional>
#include<memory>
#include<vector>

#include"core/exception.hpp"
#include"function/window/window_layer.hpp"
#include"ui/imgui_header.hpp"
#include"ui/imgui_style.hpp"
#include"ui/imgui_window.hpp"
#include"ui/ui_events.hpp"

ACDA_EXCEPTION(ImguiError);

struct ImguiLayer: iLayer
{
public:
    using self_type = ImguiLayer;
public:
    ImguiLayer(
        const std::shared_ptr<const WindowLayer>& window_layer,
        const std::function<void(ImguiLayer&)>& imgui_window_installer ={},
        const std::function<void()>& imgui_style_setter = imgui_style::default_dark
    );
    virtual ~ImguiLayer();

    [[nodiscard]]
    auto get_window() const -> std::shared_ptr<const WindowLayer>
    {
        return _window.lock();
    }

    [[nodiscard]]
    auto get_imgui_window() const -> const std::vector<std::unique_ptr<iImguiWindow>>&
    {
        return _imgui_window;
    }

    virtual void on_event(EventBase& e) override;
    virtual void on_update() override;

    template<cImguiWindow ImGuiWindow, class ...Args>
    auto emplace_imgui_window(Args&& ...args) -> self_type&
    {
        _imgui_window.emplace_back(std::make_unique<ImGuiWindow>(std::forward<Args>(args)...));
        return *this;
    }

    void scale_ui(float factor);

private:
    void _on_scale_imgui_window(events::ScaleImguiWindow& e);

public:
    bool show_demo_window{ false };
    bool show_debug_info{ false };
private:
    std::weak_ptr<const WindowLayer> _window;
    ImGuiContext* _imgui_context{ nullptr };
    std::vector<std::unique_ptr<iImguiWindow>> _imgui_window{};
};