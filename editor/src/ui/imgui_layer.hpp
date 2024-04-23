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

namespace Arcadia
{
    ARCADIA_EXCEPTION(ImguiError);

    struct ARCADIA_API ImguiLayer: Arcadia::iLayer
    {
    public:
        using self_type = ImguiLayer;
    public:
        ImguiLayer(
            const std::shared_ptr<const Arcadia::WindowLayer>& window_layer,
            const std::function<void(Arcadia::ImguiLayer&)>& imgui_window_installer ={},
            const std::function<void()>& imgui_style_setter = Arcadia::ImguiStyle::DefaultDark
        );
        virtual ~ImguiLayer();

        [[nodiscard]]
        auto GetWindow() const -> std::shared_ptr<const Arcadia::WindowLayer>
        {
            return _Window.lock();
        }

        [[nodiscard]]
        auto GetImguiWindow() const -> const std::vector<std::unique_ptr<Arcadia::iImguiWindow>>&
        {
            return _ImguiWindow;
        }

        virtual void OnEvent(Arcadia::EventBase& event) override;
        virtual void OnUpdate() override;

        template<Arcadia::cImguiWindow ImGuiWindow, class ...Args>
        auto EmplaceImguiWindow(Args&& ...args) -> self_type&
        {
            _ImguiWindow.emplace_back(std::make_unique<ImGuiWindow>(std::forward<Args>(args)...));
            return *this;
        }

    public:
        bool ShowDemoWindow{ false };
        bool ShowDebugInfo{ false };
    private:
        std::weak_ptr<const Arcadia::WindowLayer> _Window;
        ImGuiContext* _ImguiContext{ nullptr };
        std::vector<std::unique_ptr<Arcadia::iImguiWindow>> _ImguiWindow{};
    };
}
