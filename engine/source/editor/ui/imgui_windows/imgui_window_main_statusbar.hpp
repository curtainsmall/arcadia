#pragma once

#include "core/event.hpp"


#include "ui/imgui_window.hpp"

namespace Arcadia
{
    struct ImguiWindowMainStatusbar: public ImguiWindowInterface
    {
    public:
        using SelfType = ImguiWindowMainStatusbar;

        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###statusbar");

        ImguiWindowMainStatusbar():
            ImguiWindowInterface(true, "Statusbar")
        {
        }
        virtual ~ImguiWindowMainStatusbar() override = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;

    private:
        float _UiScale{};
    };
}