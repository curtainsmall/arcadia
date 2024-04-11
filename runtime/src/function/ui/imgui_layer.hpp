#pragma once

#include<functional>
#include<memory>
#include<vector>

#include"core/exception.hpp"
#include"function/ui/imgui_header.hpp"
#include"function/ui/imgui_style.hpp"
#include"function/ui/imgui_window.hpp"
#include"function/window/window_layer.hpp"

namespace Arcadia
{
    ARCADIA_EXCEPTION(ImguiError);

    struct ARCADIA_API ImguiLayer: Arcadia::iLayer
    {
    public:
        using self_type = ImguiLayer;
    public:
        ImguiLayer(
            const std::shared_ptr<const Arcadia::WindowLayer>& window_layer_sptr,
            const std::function<void(Arcadia::ImguiLayer&)>& imgui_window_installer ={},
            const std::function<void()>& imgui_style_setter = Arcadia::ImguiStyle::DefaultDark
        );
        virtual ~ImguiLayer();

        [[nodiscard]]
        auto get_window_sptr() const -> std::shared_ptr<const Arcadia::WindowLayer>
        {
            return _wpWindow.lock();
        }

        [[nodiscard]]
        auto get_imgui_window_uptrs() const -> const std::vector<std::unique_ptr<Arcadia::iImguiWindow>>&
        {
            return _upImguiWindow;
        }

        virtual void OnEvent(Arcadia::EventBase& event) override;
        virtual void OnUpdate() override;

        template<Arcadia::cImguiWindow ImGuiWindow, class ...Args>
        auto EmplaceImguiWindow(Args&& ...args) -> self_type&
        {
            _upImguiWindow.emplace_back(std::make_unique<ImGuiWindow>(std::forward<Args>(args)...));
            return *this;
        }
    public:
        bool ShowDemoWindow{ false };
        bool ShowDebugInfo{ false };
    private:
        std::weak_ptr<const Arcadia::WindowLayer> _wpWindow;
        ImGuiContext* _pImguiContext{ nullptr };
        std::vector<std::unique_ptr<Arcadia::iImguiWindow>> _upImguiWindow{};
    };
}
