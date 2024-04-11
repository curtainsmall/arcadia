#pragma once

#include"core/base.hpp"
#include"function/ui/imgui_window.hpp"

namespace Arcadia
{
    struct ARCADIA_API ImguiWindowMainToolbar: Arcadia::iImguiWindow
    {
    public:
        using self_type = ImguiWindowMainToolbar;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS("###toolbar");
    public:
        inline ImguiWindowMainToolbar():
            Arcadia::iImguiWindow(true, "Toolbar")
        {}
        virtual ~ImguiWindowMainToolbar() = default;

        virtual void OnEvent(Arcadia::EventBase& event) override;
        virtual void OnUpdate() override;
    };
}
