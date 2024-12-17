#pragma once

#include"platform/api_def.hpp"
#include"core/event/event.hpp"

#include"ui/imgui_window.hpp"

namespace Arcadia
{
    class ImguiWindowMainStatusbar: public iImguiWindow
    {
    public:
        using SelfType = ImguiWindowMainStatusbar;

        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###statusbar");

        ImguiWindowMainStatusbar() :
            iImguiWindow(true, "Statusbar")
        {}
        virtual ~ImguiWindowMainStatusbar() = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;

    private:
        float _UiScale{};
    };
}