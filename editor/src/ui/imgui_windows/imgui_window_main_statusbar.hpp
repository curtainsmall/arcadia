#pragma once

#include"core/base.hpp"
#include"core/event/event.hpp"

#include"ui/imgui_window.hpp"

struct ImguiWindowMainStatusbar: iImguiWindow
{
public:
    using self_type = ImguiWindowMainStatusbar;

    ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###statusbar");

    ImguiWindowMainStatusbar() :
        iImguiWindow(true, "Statusbar")
    {}
    virtual ~ImguiWindowMainStatusbar() = default;

    virtual void on_event(EventBase& e) override;
    virtual void on_update() override;

private:
    float _ui_scale{};
};