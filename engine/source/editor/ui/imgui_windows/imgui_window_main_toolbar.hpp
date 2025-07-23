#pragma once

#include <memory>

#include "resource/scene.hpp"
#include "resource/scene_events.hpp"

#include "project/project_events.hpp"
#include "ui/imgui_window.hpp"

namespace Arcadia
{
    struct ImguiWindowMainToolbar: public ImguiWindowInterface
    {
    public:
        using SelfType = ImguiWindowMainToolbar;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###toolbar");
    public:
        ImguiWindowMainToolbar();
        virtual ~ImguiWindowMainToolbar() override = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;
    };
}