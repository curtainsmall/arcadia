#pragma once

#include<memory>

#include"platform/api_def.hpp"
#include"resource/scene.hpp"

#include"project/project_events.hpp"
#include"ui/imgui_window.hpp"

namespace Arcadia
{
    class ImguiWindowMainToolbar: public iImguiWindow
    {
    public:
        using SelfType = ImguiWindowMainToolbar;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###toolbar");
    public:
        ImguiWindowMainToolbar() :
            iImguiWindow(true, "Toolbar")
        {}
        virtual ~ImguiWindowMainToolbar() = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;

    private:
        void _OnSceneActivated(Events::SceneActivated& e);
        void _OnSceneDeactivated(Events::SceneDeactivated& e);
    private:
        std::weak_ptr<Scene> _Scene{};
    };
}