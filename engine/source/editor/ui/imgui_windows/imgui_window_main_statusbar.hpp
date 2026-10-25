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

        ImguiWindowMainStatusbar(Engine& engine);
        virtual ~ImguiWindowMainStatusbar() override = default;

        void OnEvent(EventBase& e);
        void OnUpdate();

    private:
        float _UiScale{};
    };
}