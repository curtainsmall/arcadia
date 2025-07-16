#pragma once

#undef ERROR
#include "imgui_console/imgui_console.h"

#include "ui/imgui_window.hpp"

namespace Arcadia
{
    struct ImguiWindowConsole: public ImguiWindowInterface
    {
    public:
        using SelfType = ImguiWindowConsole;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###console");

        ImguiWindowConsole(
            bool open,
            const std::string& title
        );

        virtual ~ImguiWindowConsole() override = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;
    private:
        ImGuiConsole _ImguiConsole{};
    };
}
