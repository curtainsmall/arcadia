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
    ACDA_DEFINE_EXCEPTION(ImguiError);

    class ImguiLayer: public iLayer
    {
    public:
        using SelfType = ImguiLayer;
    public:
        ImguiLayer(
            const std::shared_ptr<const WindowLayer>& window_layer,
            const std::function<void(ImguiLayer&)>& imgui_window_installer ={},
            const std::function<void()>& imgui_style_setter = ImguiStyle::SetToDefaultDark
        );
        virtual ~ImguiLayer();

        [[nodiscard]]
        auto GetWindow() const -> std::shared_ptr<const WindowLayer>
        {
            return _Window.lock();
        }

        [[nodiscard]]
        auto GetImguiWindow() const -> const std::vector<std::unique_ptr<iImguiWindow>>&
        {
            return _ImguiWindow;
        }

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;

        template<cImguiWindow ImGuiWindow, typename ...Args>
        auto EmplaceImguiWindow(Args&& ...args) -> SelfType&
        {
            _ImguiWindow.emplace_back(std::make_unique<ImGuiWindow>(std::forward<Args>(args)...));
            return *this;
        }

        void ScaleUi(float factor);

    private:
        void _OnScaleImguiWindow(Events::ScaleImguiWindow& e);

    public:
        bool ShouldShowDemoWindow{ false };
        bool ShouldShowDebugInfo{ false };
    private:
        std::weak_ptr<const WindowLayer> _Window;
        ImGuiContext* _ImguiContext{ nullptr };
        std::vector<std::unique_ptr<iImguiWindow>> _ImguiWindow{};
    };
}