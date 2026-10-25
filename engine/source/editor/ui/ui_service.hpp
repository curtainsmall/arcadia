#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "core/exception.hpp"
#include "app/engine.hpp"
#include "function/window/window_service.hpp"
#include "ui/imgui.hpp"
#include "ui/imgui_style.hpp"
#include "ui/imgui_window.hpp"
#include "ui/ui_events.hpp"

namespace Arcadia
{
    struct UiService
    {
    public:
        using SelfType = UiService;
    public:
        UiService(
            Engine& engine,
            const std::function<void(UiService&)>& imgui_window_installer = {},
            const std::function<void()>& imgui_style_setter = ImguiStyle::SetToDefaultDark,
            float init_scale = 1
        );
        virtual ~UiService();

        [[nodiscard]]
        auto GetImguiWindows() const -> const std::vector<std::unique_ptr<ImguiWindowInterface>>&;

        void OnEvent(EventBase& e);
        void OnUpdate();

        template<Concepts::ImguiWindow ImGuiWindow, class ...Args>
        auto EmplaceImguiWindow(Args&& ...args) -> SelfType&
        {
            _ImguiWindows.emplace_back(std::make_unique<ImGuiWindow>(std::forward<Args>(args)...));
            return *this;
        }

        void ScaleUi(float factor);

    private:
        void _OnScaleImguiWindow(Events::ScaleImguiWindow& e);

    private:
        Engine& _Engine;

        Foundation::Subscription _EventSubscription{};

        ImGuiContext* _pImguiContext{ nullptr };
        std::vector<std::unique_ptr<ImguiWindowInterface>> _ImguiWindows{};
    };
}
