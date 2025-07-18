#pragma once

#include <array>
#include <string>
#include <vector>

#include "core/enum.hpp"

#include "ui/imgui.hpp"
#include "ui/imgui_window.hpp"
#include "ui/ui_events.hpp"

namespace Arcadia
{
    struct ImguiWindowConsole: public ImguiWindowInterface
    {
    public:
        using SelfType = ImguiWindowConsole;

    private:
        enum struct ColorPalette: std::int8_t
        {
            Command = 0,
            Log,
            Warning,
            Error,
            Info,

            Timestamp,

            COUNT,
        };
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
        void _OnOpenImguiWindow(Events::OpenImguiWindow& e);

        void _MenuBar();
        void _LogArea();
        void _InputBar();

        static auto _InputCallback(ImGuiInputTextCallbackData* data) -> int;

    private:
        std::string _InputBuffer{};
        ImGuiTextFilter _TextFilter{};

        bool _AutoScroll{ true };
        bool _ColoredOutput{ true };
        bool _ScrollToBottom{ true };
        bool _Timestamp{ true };

        std::array<glm::vec4, ToUnderlying(ColorPalette::COUNT)> _ColorPalette{};

        std::vector<std::string> _CmdSuggestions{};
        bool _WasPrevFrameTabCompletion{ false };
        std::size_t _HistoryIndex{};
    };
}
